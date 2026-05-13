use clap::{Parser, Subcommand};
mod commands;
use commands::{
    get_battery_threshold, get_fan_speed, get_profile, set_battery_threshold, set_profile,
};

use crate::commands::get_cpu_temp;

#[derive(Parser)]
#[command(name = "asus", about = "ASUS laptop control")]
struct Cli {
    #[command(subcommand)]
    command: Cmd,
}

#[derive(Subcommand)]
enum Cmd {
    Get {
        #[command(subcommand)]
        target: GetTarget,
    },
    Set {
        #[command(subcommand)]
        target: SetTarget,
    },
}

#[derive(Subcommand)]
enum GetTarget {
    BatteryThreshold,
    Profile,
    FanSpeed,
    CpuTemp,
}

#[derive(Subcommand)]
enum SetTarget {
    BatteryThreshold {
        value: u8,
    },
    Profile {
        profile: Profile,
    },
}

#[derive(clap::ValueEnum, Clone)]
enum Profile {
    Quiet,
    Balanced,
    Performance,
}

impl Profile {
    fn as_str(&self) -> &'static str {
        match self {
            Profile::Quiet => "quiet",
            Profile::Balanced => "balanced",
            Profile::Performance => "performance",
        }
    }
}

fn main() {
    let cli = Cli::parse();
    let result = match cli.command {
        Cmd::Get { target } => match target {
            GetTarget::BatteryThreshold => get_battery_threshold(),
            GetTarget::Profile => get_profile(),
            GetTarget::FanSpeed => get_fan_speed(),
            GetTarget::CpuTemp => get_cpu_temp(),
        },
        Cmd::Set { target } => match target {
            SetTarget::BatteryThreshold { value } => set_battery_threshold(value),
            SetTarget::Profile { profile } => set_profile(&profile),
        },
    };

    match result {
        Ok(msg) => println!("{msg}"),
        Err(e) => {
            eprintln!("error: {e}");
            std::process::exit(1);
        }
    }
}
