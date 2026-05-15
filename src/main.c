/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BATTERY_PATH "/sys/class/power_supply/BAT0/charge_control_end_threshold"
#define PROFILE_PATH "/sys/firmware/acpi/platform_profile"
#define FAN_RPM_PATH "/sys/class/hwmon/hwmon1/fan1_input"
#define CPU_TEMP_PATH "/sys/class/hwmon/hwmon8/temp1_input"

static int sysfs_read(const char *path, char *buf, size_t len) {
    FILE *f = fopen(path, "r");
    if (!f) {
        perror(path);
        return -1;
    }

    if (!fgets(buf, len, f)) {
        perror(path);
        fclose(f);
        return -1;
    }

    fclose(f);
    // strip newline
    buf[strcspn(buf, "\n")] = 0;
    return 0;
}

static int sysfs_write(const char *path, const char *val) {
    FILE *f = fopen(path, "w");
    if (!f) {
        perror(path);
        return -1;
    }

    fputs(val, f);
    fclose(f);
    return 0;
}

static void cmd_get(const char *target) {
    char buf[64];

    if (strcmp(target, "battery-threshold") == 0) {
        if (sysfs_read(BATTERY_PATH, buf, sizeof(buf)) == 0) {
            printf("%s%%\n", buf);
        }
    } else if (strcmp(target, "profile") == 0) {
        if (sysfs_read(PROFILE_PATH, buf, sizeof(buf)) == 0) {
            printf("%s\n", buf);
        }
    } else if (strcmp(target, "fan-rpm") == 0) {
        if (sysfs_read(FAN_RPM_PATH, buf, sizeof(buf)) == 0) {
            printf("%s RPM\n", buf);
        }
    } else if (strcmp(target, "cpu-temp") == 0) {
        if (sysfs_read(CPU_TEMP_PATH, buf, sizeof(buf)) == 0) {
            float milli_c = atof(buf);
            printf("%.1f°C\n", milli_c / 1000.0f);
        }
    } else {
        fprintf(stderr, "unknown target: %s\n", target);
        exit(1);
    }
}

static void cmd_set(const char *target, const char *value) {
    if (strcmp(target, "battery-threshold") == 0) {
        int v = atoi(value);
        if (v < 0 || v > 100) {
            fprintf(stderr, "value must be 0-100\n");
            exit(1);
        }
        if (sysfs_write(BATTERY_PATH, value) == 0) {
            printf("battery threshold set to %s%%\n", value);
        }
    } else if (strcmp(target, "profile") == 0) {
        if (strcmp(value, "quiet") != 0 && strcmp(value, "balanced") != 0 &&
            strcmp(value, "performance") != 0) {
            fprintf(stderr, "profile must be: quiet, balanced, performance\n");
            exit(1);
        }
        if (sysfs_write(PROFILE_PATH, value) == 0) {
            printf("profile set to %s\n", value);
        }
    } else {
        fprintf(stderr, "unknown target: %s\n", target);
        exit(1);
    }
}

static void usage(void) {
    fprintf(stderr, "usage:\n"
                    "  asus get\n"
                    "    <battery-threshold|profile|fan-rpm|cpu-temp>\n"
                    "  asus set \n"
                    "     battery-threshold <0-100>\n"
                    "     profile <quiet|balanced|performance>\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage();
    }

    if (strcmp(argv[1], "get") == 0 && argc == 3) {
        cmd_get(argv[2]);
    } else if (strcmp(argv[1], "set") == 0 && argc == 4) {
        cmd_set(argv[2], argv[3]);
    } else {
        usage();
    }

    return 0;
}