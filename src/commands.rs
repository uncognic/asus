use crate::Profile;
use std::fs;

const BATTERY_PATH: &str = "/sys/class/power_supply/BAT0/charge_control_end_threshold";
const PLATFORM_PROFILE_PATH: &str = "/sys/firmware/acpi/platform_profile";
const CPU_TEMP_PATH: &str = "/sys/class/hwmon/hwmon8/temp1_input";

pub fn read(path: &str) -> Result<String, String> {
    fs::read_to_string(path)
        .map(|s| s.trim().to_string())
        .map_err(|e| format!("failed to read {path}: {e}"))
}

pub fn write(path: &str, value: &str) -> Result<(), String> {
    fs::write(path, value).map_err(|e| format!("failed to write {path}: {e}"))
}

pub fn get_cpu_temp() -> Result<String, String> {
    let raw = read(CPU_TEMP_PATH)?;
    let milli_c: f32 = raw
        .parse()
        .map_err(|e| format!("failed to parse cpu temp: {e}"))?;

    let result = (milli_c / 1000.0).to_string();
    Ok(format!("{result} Celsius"))
}

pub fn get_battery_threshold() -> Result<String, String> {
    read(BATTERY_PATH)
}

pub fn get_profile() -> Result<String, String> {
    read(PLATFORM_PROFILE_PATH)
}

pub fn get_fan_speed() -> Result<String, String> {
    let result = read("/sys/class/hwmon/hwmon1/fan1_input")?;
    Ok(format!("{result} RPM"))
}

pub fn set_battery_threshold(value: u8) -> Result<String, String> {
    write(BATTERY_PATH, &value.to_string())?;
    Ok(format!("battery threshold set to {value}%"))
}

pub fn set_profile(profile: &Profile) -> Result<String, String> {
    write(PLATFORM_PROFILE_PATH, profile.as_str())?;
    Ok(format!("profile set to {}", profile.as_str()))
}

