# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Arduino

Arduino Wiring-based Framework allows writing cross-platform software to
control devices attached to a wide range of Arduino boards to create all
kinds of creative coding, interactive objects, spaces or physical experiences.

https://github.com/rogerclarkmelbourne/Arduino_STM32
"""

import sys
from os.path import isdir, isfile, join

from SCons.Script import DefaultEnvironment


env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = env.BoardConfig().get("build.mcu", "")

FRAMEWORK_ROOT = platform.get_package_dir("framework-arduinoststm32-maple")

if mcu.startswith("stm32f1"):
    FRAMEWORK_DIR = join(FRAMEWORK_ROOT, "STM32F1")
elif mcu.startswith("stm32f4"):
    FRAMEWORK_DIR = join(FRAMEWORK_ROOT, "STM32F4")
else:
    sys.stderr.write("Could not find a suitable framework variant\n")
    env.Exit(1)

assert isdir(FRAMEWORK_DIR)

# board -> variant
VARIANT_REMAP = {
    "disco_f407vg": "discovery_f407",
    "genericSTM32F407VET6": "generic_f407v",
    "genericSTM32F407VGT6": "generic_f407v",
    "stm32f4stamp": "discovery_f407",
    "netduino2plus": "discovery_f407"
}


def get_variant(board):
    variant = VARIANT_REMAP[board] if board in VARIANT_REMAP else board
    return variant


def process_optimization_level(cpp_defines):
    if "OPTIMIZATION_FAST" in cpp_defines:
        env.Append(CCFLAGS=["-O1"])
    elif "OPTIMIZATION_FASTER" in cpp_defines:
        env.Append(CCFLAGS=["-O2"])
    elif "OPTIMIZATION_FASTEST" in cpp_defines:
        env.Append(CCFLAGS=["-O3"])
    else:
        env.Append(CCFLAGS=["-Os"])  # default optimize for size


def process_usb_configuration(cpp_defines):
    if "ENABLE_USB_MASS_STORAGE" in cpp_defines:
        env.Append(CPPDEFINES=["USB_MSC"])
    elif "ENABLE_USB_SERIAL" in cpp_defines:
        env.Append(CPPDEFINES=["SERIAL_USB"])
    else:
        # USB is disabled by default
        env.Append(CPPDEFINES=["USB_NC"])


def configure_application_offset(board_name, upload_protocol):
    addr = "0x08000000"
    # Handle board without bootloader
    if not board_name.startswith("generic"):
        return addr
    if upload_protocol in ("dfu", "hid"):
        addr = "0x08004000"
    if upload_protocol == "hid":
        env.Append(CPPDEFINES=["VECT_TAB_OFFSET", "0x4000"])
    env.Append(CPPDEFINES=[("USER_ADDR_ROM", '"(uint32)%s"' % addr)])


def get_linker_script(board_name, upload_protocol):
    # Only generic board has custom offset
    if board_name.startswith("generic") and upload_protocol in ("dfu", "hid"):
        return "bootloader_8004000.ld"
    return "jtag.ld"


def configure_error_led(board):

    if board.startswith("genericSTM32F407"):
        led_port = "GPIOA"
        led_pin = 7
    else:
        # default for all boards
        led_port = "GPIOD"
        led_pin = 14

    env.Append(
        CPPDEFINES=[
            ("ERROR_LED_PORT", led_port),
            ("ERROR_LED_PIN", led_pin)
        ]
    )

board_name = env.subst("$BOARD")
variant = get_variant(board_name)
variant_dir = join(FRAMEWORK_DIR, "variants", variant)
mcu_family = mcu[0:7].upper()
upload_protocol = env.subst("$UPLOAD_PROTOCOL")

env.Append(
    ASFLAGS=["-x", "assembler-with-cpp"],

    CFLAGS=["-std=gnu11"],

    CCFLAGS=[
        "-mcpu=%s" % env.BoardConfig().get("build.cpu"),
        "-mthumb",
        "-ffunction-sections",  # place each function in its own section
        "-fdata-sections",
        "-nostdlib",
        "--param", "max-inline-insns-single=500",
        "-Wall"
    ],

    CXXFLAGS=[
        "-std=gnu++11",
        "-fno-rtti",
        "-fno-exceptions"
    ],

    CPPDEFINES=[
        ("ARDUINO", 10808),
        "ARDUINO_ARCH_%s" % mcu_family,
        "__%s__" % mcu_family,
        "BOARD_%s" % variant,
        ("F_CPU", "$BOARD_F_CPU"),
        "STM32_HIGH_DENSITY",
        "STM32F4",
        "VECT_TAB_FLASH"
    ]
)

env.Append(
    ASFLAGS=env.get("CCFLAGS", [])[:],

    CPPPATH=[
        join(FRAMEWORK_DIR, "cores", "maple"),
        join(FRAMEWORK_DIR, "cores", "maple", "libmaple"),
        join(FRAMEWORK_DIR, "cores", "maple", "libmaple", "usbF4"),
        join(FRAMEWORK_DIR, "cores", "maple", "libmaple", "usbF4", "VCP"),
        join(FRAMEWORK_DIR, "system", "libmaple"),
    ],

    LINKFLAGS=[
        "-Os",
        "-mthumb",
        "-mcpu=%s" % env.BoardConfig().get("build.cpu"),
        "-Wl,--gc-sections"
    ],

    LIBS=["m", "gcc"],

    LIBPATH=[
        join(FRAMEWORK_DIR, "variants", variant, "ld")
    ],

    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries")
    ]
)

if not board.get("build.ldscript", ""):
    ldscript = get_linker_script(board_name, upload_protocol)
    if not isfile(join(FRAMEWORK_DIR, "variants", variant, "ld", ldscript)):
        print("Warning! Cannot find linker script for the current target!\n")
    env.Replace(LDSCRIPT_PATH=ldscript)

configure_error_led(board_name)
configure_application_offset(board_name, upload_protocol)

cpp_defines = env.Flatten(env.get("CPPDEFINES", []))
process_optimization_level(cpp_defines)
process_usb_configuration(cpp_defines)

#
# Target: Build Core Library
#

if "build.variant" in env.BoardConfig():
    env.Append(
        CPPPATH=[variant_dir]
    )

    env.BuildSources(
        join("$BUILD_DIR", "FrameworkArduinoVariant"),
        variant_dir)


env.BuildSources(
    join("$BUILD_DIR", "FrameworkArduino"),
    join(FRAMEWORK_DIR, "cores", "maple"))
