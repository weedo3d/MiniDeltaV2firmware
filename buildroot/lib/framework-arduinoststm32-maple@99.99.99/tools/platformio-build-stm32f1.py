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
    "bluepill_f103c8": "generic_stm32f103c",
    "bluepill_f103c6": "generic_stm32f103c",
    "bluepill_f103c8_128k": "generic_stm32f103c",
    "maple_mini_b20": "maple_mini",
    "maple_mini_origin": "maple_mini",
    "microduino32_flash": "microduino",
    "genericSTM32F103C8": "generic_stm32f103c",
    "genericSTM32F103CB": "generic_stm32f103c",
    "genericSTM32F103R8": "generic_stm32f103r8",
    "genericSTM32F103RB": "generic_stm32f103r8",
    "genericSTM32F103RC": "generic_stm32f103r",
    "genericSTM32F103RE": "generic_stm32f103r",
    "genericSTM32F103T8": "generic_stm32f103t",
    "genericSTM32F103TB": "generic_stm32f103t",
    "genericSTM32F103VB": "generic_stm32f103vb",
    "genericSTM32F103VC": "generic_stm32f103v",
    "genericSTM32F103VD": "generic_stm32f103v",
    "genericSTM32F103VE": "generic_stm32f103v",
    "genericSTM32F103ZC": "generic_stm32f103z",
    "genericSTM32F103ZD": "generic_stm32f103z",
    "genericSTM32F103ZE": "generic_stm32f103z",
    "disco_f100rb": "STM32VLD"
}


def get_variant(board):
    variant = VARIANT_REMAP[board] if board in VARIANT_REMAP else board
    return variant


def get_vector_tab_addr(board, upload_protocol):
    # Some special cases
    boards_8005000 = (
        "maple",
        "maple_ret6",
        "microduino32_flash",
        "maple_mini_origin"
    )

    if any(b == board for b in boards_8005000):
        return "0x8005000"

    # depending on upload method
    if upload_protocol == "dfu":
        return "0x8002000"
    elif upload_protocol == "hid":
        return "0x8001000"
    else:
        # for other upload methods (including serial)
        return "0x8000000"


def process_optimization_level(cpp_defines):
    if "OPTIMIZATION_FAST" in cpp_defines:
        env.Append(CCFLAGS=["-O1"])
    elif "OPTIMIZATION_FASTER" in cpp_defines:
        env.Append(CCFLAGS=["-O2"])
    elif "OPTIMIZATION_FASTEST" in cpp_defines:
        env.Append(CCFLAGS=["-O3"])
    else:
        env.Append(CCFLAGS=["-Os"])  # default optimize for size


def add_upload_protocol_defines(board, upload_protocol):
    if upload_protocol == "serial":
        env.Append(
            CPPDEFINES=[("CONFIG_MAPLE_MINI_NO_DISABLE_DEBUG", 1)])
    elif upload_protocol == "dfu":
        env.Append(CPPDEFINES=["SERIAL_USB"])
    else:
        env.Append(
            CPPDEFINES=[
                ("CONFIG_MAPLE_MINI_NO_DISABLE_DEBUG", 1),
                "SERIAL_USB"
            ])

    is_generic = board.startswith("generic") or board == "hytiny_stm32f103t"
    if upload_protocol in ("stlink", "dfu", "jlink") and is_generic:
        env.Append(CPPDEFINES=["GENERIC_BOOTLOADER"])


def get_linker_script(board, mcu, upload_protocol):
    if upload_protocol == "dfu":
        boards_with_boot_20 = (
            "genericSTM32F103C",
            "genericSTM32F103T",
            "maple_mini_b20",
            "hytiny_stm32f103t"
        )

        if any(b in board for b in boards_with_boot_20):
            return "bootloader_20.ld"
        elif board.startswith("bluepill_f103c8"):
            return "bootloader_20.ld"
        elif board.startswith("genericSTM32F103R"):
            return "bootloader.ld"
        elif board.startswith("genericSTM32F103V"):
            return "stm32f103veDFU.ld"
        elif board.startswith("genericSTM32F103Z"):
            return "stm32f103z_dfu.ld"
        elif board == "maple_ret6":
            return "stm32f103re-bootloader.ld"

    elif upload_protocol == "hid":
        # a rare case only one board
        if board.startswith("genericSTM32F103C"):
            return "hid_bootloader.ld"
        else:
            sys.stderr.write(
                "%s doesn't support uploading using HID method\n" % board)
            env.Exit(1)
    else:
        # Any other upload protocol
        specific_scripts = (
            "genericSTM32F103R",
            "genericSTM32F103V",
            "genericSTM32F103Z"
        )

        flash_scripts = (
            "maple",
            "mapleMini",
            "microduino32_flash"
        )

        jtag_scripts = (
            "nucleo_f103rb",
            "genericSTM32F103CB",
            "genericSTM32F103TB",
            "disco_f100rb",
            "hytiny_stm32f103t"
        )

        if any(b in board for b in specific_scripts):
            return "%s.ld" % mcu[0:11]
        elif any(b in board for b in flash_scripts):
            return "flash.ld"
        elif any(b in board for b in jtag_scripts):
            return "jtag.ld"
        elif board == "genericSTM32F103C8":
            return "jtag_c8.ld"
        elif board == "genericSTM32F103T8":
            return "jtag_t8.ld"

    return "flash.ld"


def configure_error_led(board):

    if board.startswith("genericSTM32F103C"):
        led_port = "GPIOC"
        led_pin = 13
    elif board.startswith("genericSTM32F103V"):
        led_port = "GPIOE"
        led_pin = 6
    else:
        # default for all boards
        led_port = "GPIOB"
        led_pin = 1

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
        "-march=armv7-m",
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
        "ARDUINO_ARCH_STM32",
        "ARDUINO_ARCH_%s" % mcu_family,
        "ARDUINO_%s" % (variant[0:18].upper() if board_name.startswith(
            "generic") else variant.upper()),
        "MCU_%s" % mcu[0:11].upper(),
        "__%s__" % mcu_family,
        "BOARD_%s" % variant,
        ("F_CPU", "$BOARD_F_CPU"),
        ("VECT_TAB_ADDR", get_vector_tab_addr(board_name, upload_protocol))
    ]
)

env.Append(
    ASFLAGS=env.get("CCFLAGS", [])[:],

    CPPPATH=[
        join(FRAMEWORK_DIR, "cores", "maple"),
        join(FRAMEWORK_DIR, "system", "libmaple"),
        join(FRAMEWORK_DIR, "system", "libmaple", "include"),
        join(FRAMEWORK_DIR, "system", "libmaple", "stm32f1", "include"),
        join(FRAMEWORK_DIR, "system", "libmaple", "usb", "stm32f1"),
        join(FRAMEWORK_DIR, "system", "libmaple", "usb", "usb_lib"),
    ],

    LINKFLAGS=[
        "-Os",
        "-mthumb",
        "-mcpu=%s" % env.BoardConfig().get("build.cpu"),
        "-Wl,--check-sections",
        "-Wl,--gc-sections",
        "-Wl,--unresolved-symbols=report-all",
        "-Wl,--warn-common",
        "-Wl,--warn-section-align",
        "-Wl,--warn-unresolved-symbols"
    ],

    LIBS=["m", "gcc"],

    LIBPATH=[
        join(FRAMEWORK_DIR, "variants", variant, "ld")
    ],

    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries")
    ]
)

# remap ldscript
if not board.get("build.ldscript", ""):
    ldscript = get_linker_script(board_name, mcu, upload_protocol)
    if not isfile(join(FRAMEWORK_DIR, "variants", variant, "ld", ldscript)):
        print("Warning! Cannot find linker script for the current target!\n")
    env.Replace(LDSCRIPT_PATH=ldscript)

configure_error_led(board_name)
process_optimization_level(env['CPPDEFINES'])
add_upload_protocol_defines(board_name, upload_protocol)

# By default nucleo_f103rb is overclocked
if board_name == "nucleo_f103rb" and env.subst("$BOARD_F_CPU") >= 72000000:
    env.Append(CPPDEFINES=["NUCLEO_HSE_CRYSTAL"])

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
