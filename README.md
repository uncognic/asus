# asus
Simple command line tool for fetching and setting system info on ASUS laptops
## Install
1. [Install Rust](https://rust-lang.org/tools/install/)
2. Run `sudo env PATH=$PATH HOME=$HOME cargo install --path . --root /usr/local`
This installs it to /usr/local/bin/asus, so you can use it anywhere.
## Features
### Get
- Battery charge limit
- Performance/fan profile
- Fan speed RPM
- CPU temperature celsius
### Set
- Battery charge limit
- Performance/fan profile
## Support
Tested on:
- ASUS Vivobook S16 M3607HA

If you have another laptop and it doesn't work, you probably have different /sys/class paths. Contact me and I can add them, or create a pull request if you can do it yourself.
