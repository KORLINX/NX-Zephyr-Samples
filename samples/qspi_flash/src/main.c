/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <string.h>

#if DT_HAS_COMPAT_STATUS_OKAY(jedec_spi_nor)
#define SPI_FLASH_COMPAT jedec_spi_nor
#elif DT_HAS_COMPAT_STATUS_OKAY(nordic_qspi_nor)
#define SPI_FLASH_COMPAT nordic_qspi_nor
#elif DT_HAS_COMPAT_STATUS_OKAY(st_stm32_qspi_nor)
#define SPI_FLASH_COMPAT st_stm32_qspi_nor
#elif DT_HAS_COMPAT_STATUS_OKAY(st_stm32_ospi_nor)
#define SPI_FLASH_COMPAT st_stm32_ospi_nor
#elif DT_HAS_COMPAT_STATUS_OKAY(st_stm32_xspi_nor)
#define SPI_FLASH_COMPAT st_stm32_xspi_nor
#else
#define SPI_FLASH_COMPAT invalid
#endif

/* P25Q32SU: 32Mbit = 4MB. DT size prop is in bits. */
#define FLASH_SIZE    (DT_PROP(DT_INST(0, SPI_FLASH_COMPAT), size) / 8U)
#define SECTOR_SIZE   4096U
#define PAGE_SIZE     256U
#define NUM_SECTORS   (FLASH_SIZE / SECTOR_SIZE)
#define NUM_PAGES     (FLASH_SIZE / PAGE_SIZE)

/* Speed test region: first 256 KB */
#define SPEED_TEST_SIZE     (256U * 1024U)
#define SPEED_TEST_SECTORS  (SPEED_TEST_SIZE / SECTOR_SIZE)
#define SPEED_TEST_PAGES    (SPEED_TEST_SIZE / PAGE_SIZE)

/* Progress print interval */
#define PROGRESS_SECTORS  64U
#define PROGRESS_PAGES    512U

static uint8_t wbuf[PAGE_SIZE];
static uint8_t rbuf[PAGE_SIZE];

static void fill_pattern(uint8_t *buf, size_t len, uint32_t offset)
{
	for (size_t i = 0; i < len; i++) {
		buf[i] = (uint8_t)((offset + i) & 0xFF);
	}
}

static void print_speed(const char *label, uint32_t bytes, int64_t ms)
{
	if (ms <= 0) {
		printk("  %-22s  %u bytes  <1 ms\n", label, bytes);
		return;
	}
	/* KB/s = (bytes / 1024) / (ms / 1000) = bytes * 1000 / (ms * 1024) */
	uint32_t kbps = (uint32_t)((uint64_t)bytes * 1000U / ((uint64_t)ms * 1024U));

	printk("  %-22s  %5u KB  %6lld ms  %5u KB/s\n",
	       label, bytes / 1024U, ms, kbps);
}

/* Phase 1: erase every sector */
static int phase_erase(const struct device *dev, int64_t *elapsed_ms)
{
	uint32_t fail = 0;
	int rc;

	printk("\n[Phase 1] Erasing %u sectors (%u KB)...\n",
	       NUM_SECTORS, FLASH_SIZE / 1024U);

	int64_t t0 = k_uptime_get();

	for (uint32_t s = 0; s < NUM_SECTORS; s++) {
		uint32_t off = s * SECTOR_SIZE;

		rc = flash_erase(dev, off, SECTOR_SIZE);
		if (rc != 0) {
			printk("  ERASE FAIL  sector %4u  offset 0x%06x  rc=%d\n",
			       s, off, rc);
			fail++;
		} else if (s % PROGRESS_SECTORS == 0) {
			printk("  erased sector %4u / %u  (0x%06x)\n",
			       s, NUM_SECTORS, off);
		}
	}

	*elapsed_ms = k_uptime_get() - t0;
	printk("[Phase 1] done — %u erase errors\n", fail);
	return fail ? -1 : 0;
}

/* Phase 2: read back every page and confirm 0xFF */
static int phase_verify_erase(const struct device *dev, int64_t *elapsed_ms)
{
	uint32_t fail = 0;
	int rc;

	printk("\n[Phase 2] Verifying erase across %u pages...\n", NUM_PAGES);

	int64_t t0 = k_uptime_get();

	for (uint32_t p = 0; p < NUM_PAGES; p++) {
		uint32_t off = p * PAGE_SIZE;

		memset(rbuf, 0x00, PAGE_SIZE);
		rc = flash_read(dev, off, rbuf, PAGE_SIZE);
		if (rc != 0) {
			printk("  READ FAIL   page %5u  offset 0x%06x  rc=%d\n",
			       p, off, rc);
			fail++;
			continue;
		}

		for (size_t i = 0; i < PAGE_SIZE; i++) {
			if (rbuf[i] != 0xFF) {
				printk("  ERASE FAIL  page %5u  offset 0x%06x"
				       "  byte[%3zu]=0x%02x (want 0xFF)\n",
				       p, off, i, rbuf[i]);
				fail++;
				break;
			}
		}

		if (p % PROGRESS_PAGES == 0) {
			printk("  verified page %5u / %u  (0x%06x)\n",
			       p, NUM_PAGES, off);
		}
	}

	*elapsed_ms = k_uptime_get() - t0;
	printk("[Phase 2] done — %u verify errors\n", fail);
	return fail ? -1 : 0;
}

/* Phase 3: write pattern page-by-page, then read back and verify */
static int phase_write_verify(const struct device *dev, int64_t *elapsed_ms)
{
	uint32_t write_fail = 0;
	uint32_t verify_fail = 0;
	int64_t t_write, t_verify;
	int rc;

	printk("\n[Phase 3] Writing pattern to %u pages...\n", NUM_PAGES);

	int64_t t0 = k_uptime_get();

	for (uint32_t p = 0; p < NUM_PAGES; p++) {
		uint32_t off = p * PAGE_SIZE;

		fill_pattern(wbuf, PAGE_SIZE, off);
		rc = flash_write(dev, off, wbuf, PAGE_SIZE);
		if (rc != 0) {
			printk("  WRITE FAIL  page %5u  offset 0x%06x  rc=%d\n",
			       p, off, rc);
			write_fail++;
		} else if (p % PROGRESS_PAGES == 0) {
			printk("  wrote page %5u / %u  (0x%06x)\n",
			       p, NUM_PAGES, off);
		}
	}

	t_write = k_uptime_get() - t0;
	printk("  write done — %u errors\n\n", write_fail);
	printk("[Phase 3] Verifying %u pages...\n", NUM_PAGES);

	t0 = k_uptime_get();

	for (uint32_t p = 0; p < NUM_PAGES; p++) {
		uint32_t off = p * PAGE_SIZE;

		fill_pattern(wbuf, PAGE_SIZE, off);
		memset(rbuf, 0x00, PAGE_SIZE);

		rc = flash_read(dev, off, rbuf, PAGE_SIZE);
		if (rc != 0) {
			printk("  READ FAIL   page %5u  offset 0x%06x  rc=%d\n",
			       p, off, rc);
			verify_fail++;
			continue;
		}

		if (memcmp(wbuf, rbuf, PAGE_SIZE) != 0) {
			for (size_t i = 0; i < PAGE_SIZE; i++) {
				if (wbuf[i] != rbuf[i]) {
					printk("  MISMATCH    page %5u"
					       "  offset 0x%06x"
					       "  byte[%3zu]"
					       "  wrote 0x%02x  got 0x%02x\n",
					       p, off, i, wbuf[i], rbuf[i]);
					break;
				}
			}
			verify_fail++;
		} else if (p % PROGRESS_PAGES == 0) {
			printk("  verified page %5u / %u  (0x%06x)\n",
			       p, NUM_PAGES, off);
		}
	}

	t_verify = k_uptime_get() - t0;
	*elapsed_ms = t_write + t_verify;

	printk("[Phase 3] done — %u write errors, %u verify errors\n",
	       write_fail, verify_fail);
	return (write_fail || verify_fail) ? -1 : 0;
}

/* Phase 4: dedicated speed test over first 256 KB */
static void phase_speed(const struct device *dev)
{
	int64_t t0, elapsed;
	int rc;

	printk("\n[Phase 4] Speed test — %u KB region (offset 0x000000)\n",
	       SPEED_TEST_SIZE / 1024U);

	/* --- Erase speed --- */
	t0 = k_uptime_get();
	for (uint32_t s = 0; s < SPEED_TEST_SECTORS; s++) {
		rc = flash_erase(dev, s * SECTOR_SIZE, SECTOR_SIZE);
		if (rc != 0) {
			printk("  erase error at sector %u rc=%d\n", s, rc);
		}
	}
	elapsed = k_uptime_get() - t0;
	print_speed("Erase (sector-by-sector)", SPEED_TEST_SIZE, elapsed);

	/* --- Write speed --- */
	t0 = k_uptime_get();
	for (uint32_t p = 0; p < SPEED_TEST_PAGES; p++) {
		uint32_t off = p * PAGE_SIZE;

		fill_pattern(wbuf, PAGE_SIZE, off);
		rc = flash_write(dev, off, wbuf, PAGE_SIZE);
		if (rc != 0) {
			printk("  write error at page %u rc=%d\n", p, rc);
		}
	}
	elapsed = k_uptime_get() - t0;
	print_speed("Write (page-by-page)", SPEED_TEST_SIZE, elapsed);

	/* --- Read speed --- */
	t0 = k_uptime_get();
	for (uint32_t p = 0; p < SPEED_TEST_PAGES; p++) {
		rc = flash_read(dev, p * PAGE_SIZE, rbuf, PAGE_SIZE);
		if (rc != 0) {
			printk("  read error at page %u rc=%d\n", p, rc);
		}
	}
	elapsed = k_uptime_get() - t0;
	print_speed("Read  (page-by-page)", SPEED_TEST_SIZE, elapsed);

	printk("[Phase 4] done\n");
}


int main(void)
{
	const struct device *flash_dev = DEVICE_DT_GET_ONE(SPI_FLASH_COMPAT);

	if (!device_is_ready(flash_dev)) {
		printk("%s: device not ready.\n", flash_dev->name);
		return 0;
	}

	printk("\n%s full flash test\n", flash_dev->name);
	printk("  size    : %u KB (%u bytes)\n",
	       FLASH_SIZE / 1024U, FLASH_SIZE);
	printk("  sectors : %u x %u B\n", NUM_SECTORS, SECTOR_SIZE);
	printk("  pages   : %u x %u B\n", NUM_PAGES, PAGE_SIZE);
	printk("==================================\n");

	int64_t t1_ms, t2_ms, t3_ms;

	int r1 = phase_erase(flash_dev, &t1_ms);
	int r2 = phase_verify_erase(flash_dev, &t2_ms);
	int r3 = phase_write_verify(flash_dev, &t3_ms);

	phase_speed(flash_dev);

	printk("\n==================================\n");
	printk("Results:\n");
	printk("  Phase 1 erase        : %s  (%lld ms)\n",
	       r1 == 0 ? "PASS" : "FAIL", t1_ms);
	printk("  Phase 2 erase verify : %s  (%lld ms)\n",
	       r2 == 0 ? "PASS" : "FAIL", t2_ms);
	printk("  Phase 3 write/verify : %s  (%lld ms)\n",
	       r3 == 0 ? "PASS" : "FAIL", t3_ms);
	printk("  Overall              : %s\n",
	       (r1 == 0 && r2 == 0 && r3 == 0) ? "PASS" : "FAIL");

	printk("\nThroughput summary:\n");
	print_speed("Phase 1 erase (4 MB)",  FLASH_SIZE, t1_ms);
	print_speed("Phase 2 read  (4 MB)",  FLASH_SIZE, t2_ms);
	print_speed("Phase 3 write (4 MB)",  FLASH_SIZE, t3_ms);

	return 0;
}
