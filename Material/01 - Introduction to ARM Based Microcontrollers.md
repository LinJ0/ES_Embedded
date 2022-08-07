01 - Introduction to ARM Based Microcontrollers
===

---

## Background of ARM and ARM Architecture
### A Brief History
1. ARM was formed in 1990 as Advanced RISC Machines Ltd., a joint venture of Apple Computer, Acorn Computer Group, and VLSI Technology.

2. In 1991, ARM introduced the **ARM6** processor family, and VLSI became the initial licensee. Subsequently, additional companies, including Texas Instruments, NEC, Sharp, and ST Microelectronics, licensed the ARM processor designs, extending the applications of ARM processors into mobile phones, computer hard disks, personal digital assistants (PDAs), home entertainment systems, and many other consumer products.

3. **ARM does not manufacture processors or sell the chips directly**. Instead, ARM licenses the processor designs to business partners, including a majority of the world’s leading semiconductor companies. Based on the ARM low-cost and power-efficient processor designs, these partners create their processors, microcontrollers, and system-on-chip solutions. This business model is commonly called **intellectual property (IP)** licensing.

### ARM Processor Evolution
1. One of the most successful processor products from ARM the **ARM7TDMI** processor, which is used in many 32-bit microcontrollers around the world. Unlike traditional 32-bit processors, the ARM7TDMI supports two instruction sets, one called the **ARM** instruction set with 32-bit instructions, and another 16-bit instruction set called **Thumb**.

2. By allowing both instruction sets to be used on the processor, the code density is greatly increased, hence reducing the memory footprint of the application code.

3. Since then, ARM has continued to develop new processors to address the needs of different applications. For example:
    * The **ARM9** processor family is used in a large number of high-performance microcontrollers.
    * The **ARM11** processor family is used in a large number of mobile phones.

4. Following the introduction of the ARM11 family, it was decided that many of the new technologies, such as the optimized **Thumb-2 instruction set**, were just as applicable to the lower cost markets of microcontrollers and automotive components.

5. It was also decided that although the architecture needed to be consistent from the lowest MCU to the highest performance application processor, there was a need to deliver processor architectures that best fit applications, enabling very deterministic and low gate count processors for cost-sensitive markets, and feature-rich and high-performance ones for highend applications.

6. Over the past few years, ARM has extended its product portfolio by diversifying its CPU development, which resulted in the new processor family name “Cortex.” In this Cortex processor range, the processors are divided into three profiles:

    ![ARM Cortex-A, R, M](http://125.227.181.237/uploads/upload_3be995c2f0ec29989a38f710f471f729.png)
    ref: https://www.anandtech.com/show/10049/arm-announces-cortex-r8

    * The **A** profile is designed for high-performance open **application platforms**.
    * The **R** profile is designed for high-end embedded systems in which **real-time performance** is needed.
    * The **M** profile is designed for deeply embedded **microcontroller-type systems**.

    ![Diversity of processor products for three areas in the Cortex processor family](http://125.227.181.237/uploads/upload_2bd87cf88dc5a650e4c04e246f8c82b5.png)


#### Cortex-A
Application processors that are designed to handle complex applications such as high-end **embedded operating systems (OSs)** (e.g., iOS, Android, Linux, and Windows). These applications require the highest processing power, **virtual memory system support with memory management units (MMUs)**, and, optionally, enhanced Java support and a secure program execution environment. Example products include high-end smartphones, tablets, televisions, and even computing servers.

#### Cortex-R
Real-time, high-performance processors targeted primarily at **the higher end of the real-time market** ─ these are applications, such as hard drive controllers, baseband controllers for mobile communications, and automotive systems, in which **high processing power** and **high reliability** are essential and for which **low latency** and **determinism** are important.

#### Cortex-M
Processors targeting smaller scale applications such as microcontrollers and mixed signal design, where criteria like **low cost**, **low power**, **energy efficiency**, and **low interrupt latency** are important. At the same time, the processor design has to be easy to use and able to **provide deterministic behavior as required in many real-time control systems**.


### Architecture Versions and Thumb ISA
1. ARM develops new processors, new instructions, and architectural features are added from time to time. As a result, there are different versions of the architecture.

    ![ARM Architecture](http://125.227.181.237/uploads/upload_2e7c59f81a3c39b28c940f11f179f279.png)


2. The successful ARM7TDMI is based on the architecture version ARMv4T (The “T” means Thumb instruction support). Note that architecture version numbers are independent of processor names.

3. In order to address different needs of a wide range of application areas, architecture version 7 is divided into three profiles:
    * Cortex-A Processors: ARMv7-A Architecture
    * Cortex-R Processors: ARMv7-R Architecture
    * Cortex-M Processors: ARMv7-M Architectures

    ![ARM Architecture with products](http://125.227.181.237/uploads/upload_8d353a794b32bef243f614474b4a88a6.png)


4. All the Cortex-M processors support Thumb-2 technology and support different subsets of the Thumb ISA (Instruction Set Architecture).

5. Before Thumb-2 Technology was available, the Thumb ISA was a 16-bit only instruction set.

6. Thumb-2 technology extended the Thumb ISA into a highly efficient and powerful instruction set that delivers significant benefits in terms of ease of use, code size, and performance.

7. According to 《ARM Architecture Reference Manual Thumb-2 Supplement》:
    >Thumb-2 extends the Thumb architecture by adding the following:
    >* A substantial number of **new 32-bit Thumb instructions**. These cover most of the functionality of the ARM instruction set. The main omission is the absence of a condition field in almost all Thumb instructions.
    >* Several new 16-bit Thumb instructions. One of these, the IT (If Then) instruction, provides an efficient alternative mechanism for conditional execution.

    >Thumb-2 also extends the ARM ISA by adding a small number of new ARM instructions, and some additional variants of the ARM LDR and STR instructions. The additions provide ARM equivalents of instructions supported in the Thumb instruction set.

    ![The relationship between the Thumb instruction set and the instruction set implemented in the Cortex-M processors](http://125.227.181.237/uploads/upload_169ef17a7321786056d15069b361cd8e.png)


8. With support for both 16-bit and 32-bit instructions in the Thumb-2 instruction set, there is no need to switch the processor between Thumb state (16-bit instructions) and ARM state (32-bit instructions).

    ![Instruction set comparison between Cortex-M processors and ARM7TDMI](http://125.227.181.237/uploads/upload_2e204b895837feccfc2c0a7b04d7c7f7.png)


---

## ARM Single Board Computers
### Raspberry Pi

![raspberrypi](http://125.227.181.237/uploads/upload_5c50a90ab125244f4cbb352c36c7186a.png)
Raspberry Pi 2, ref: https://www.raspberrypi.org/products/raspberry-pi-2-model-b/

#### SoC: Broadcom BCM283x

![](http://125.227.181.237/uploads/upload_8a3c45b47e18cdd1dc5c7fa9b803077a.png)

ref: https://en.wikipedia.org/wiki/Raspberry_Pi

### BeagleBoard

![](http://125.227.181.237/uploads/upload_03145eea1af41d8941605c026399cf86.png)
BeagleBone Black, ref: https://beagleboard.org/black

#### SoC: Texas Instruments AM3358 Sitara Processor

![](http://125.227.181.237/uploads/upload_55836cfc753a342fb63b8615a2685586.png)

![](http://125.227.181.237/uploads/upload_9107f3f4f27be15aaefce0b7d489d432.png)

ref: http://www.ti.com/processors/sitara-arm/am335x-cortex-a8/overview.html

---

## ARM Microcontrollers
### Well-known Microcontrollers Based on the Cortex-M
#### STMicroelectronics (意法半導體) STM32F4 Series

![](http://125.227.181.237/uploads/upload_8fa6db89f63fcc30da9373a95892c0fa.png)

![](http://125.227.181.237/uploads/upload_516cdb33c9558b4e5b2e99ec07b6cb47.png)

#### Texas Instruments (德州儀器) Tiva C Series

![](http://125.227.181.237/uploads/upload_d440f7e2752333cb3559427b7f8bf25e.png)

![](http://125.227.181.237/uploads/upload_461a0a7179fbb56165936aaa42957192.png)

>Texas Instrument's Tiva™ C Series microcontrollers provide designers a high-performance ARM® Cortex™-M-based architecture with a broad set of integration capabilities and a strong ecosystem of software and development tools. Targeting performance and flexibility, the Tiva™ C Series architecture offers a 120 MHz Cortex-M with FPU, a variety of integrated memories and multiple programmable GPIO.

ref:
* https://www.microcontrollertips.com/tis-gateway-cloud-connected-products-tiva-c-series/
* http://www.ti.com/product/TM4C129ENCPDT


### What are Inside Typical ARM Microcontrollers?
There are many different things inside a microcontroller. In many microcontrollers, the processor takes less than 10% of the silicon area, and the rest of the silicon die is occupied by other components such as:
* Program memory (e.g., flash memory)
* SRAM
* Peripherals
* Internal bus infrastructure
* Clock generator (including Phase Locked Loop), reset generator, and distribution network for these signals
* Voltage regulator and power control circuits
* Other analog components (e.g., ADC, DAC, voltage reference circuits)
* I/O pads
* Support circuits for manufacturing tests, etc.

![A simple Cortex-M3 or Cortex-M4 processor based system](http://125.227.181.237/uploads/upload_13144a5e0cfb94c2d59f2fc2ae44dd5f.png)

![Block diagram of the Cortex-M3 and Cortex-M4 processor](http://125.227.181.237/uploads/upload_e7e05094d70981522ea0b14abb40edc6.png)


### Development Boards
#### STMicroelectronics STM32F4 DISCOVERY Series Development Boards & Kits

![](http://125.227.181.237/uploads/upload_4a756d342ded8b527be93a2c76725a53.png)
ref: https://www.st.com/en/evaluation-tools/stm32f4discovery.html

#### Crypto Connected LaunchPad Evaluation Kit

![](http://125.227.181.237/uploads/upload_9da7da113c9d93ecb4172af933e32feb.png)
ref: http://www.ti.com/tool/EK-TM4C129EXL


-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`
