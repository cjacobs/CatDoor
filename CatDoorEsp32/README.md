# CatDoor

## Testing
[PlatformIO testing docs](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)



## How to execute a native program

```bash
> pio run -t exec

# extra args
> pio run -t exec --program-arg "arg1" --program-arg="--option1=value1"

# or short format
> pio run -t exec -a "arg1" -a "--option1=value1"
```

Example of how to run the "Hello World" example:
```bash
# Change directory to example
$ cd platform-native/examples/hello-world

# Build project
$ pio run

# Execute program
$ pio run --target exec
# or using embedded's "upload" target
$ pio run --target upload

# Clean build files
$ pio run --target clean
```

Testing example:
```bash
# Change directory to example
> cd platformio-examples/unit-testing/calculator

# Test project
> platformio test

# Test specific environment
> platformio test -e uno

# Process test on native desktop machine
> platformio test -e native
```