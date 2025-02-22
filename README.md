# Console IO

The console IO module can be used to add a simple user input/output console to a project.

The current implementation is a usart IRQ managed input/output. The implementatin does not suppor DMA transfer.

## Adding the module to a project

The console IO module can be added to a project by cloning it in your project

git clone 

Code location in your project can be anything but we suggest under /<Project>/Components/console-io

The console module also depends on a FIFO module that also needs to be cloned in your project:

git clone https://github.com/claudecoache/console-io.git
git clone https://github.com/claudecoache/fifo.git

Code location in your project can be anything but we suggest under /<Project>/Components/fifo

These 2 modules can also be set as submodules if you prefer.

## Console IO configuration

There is also some parameters that can be configured.

In the .ioc of the project, define these parameters:

CONSOLE_USART: This parameter is used in the main to initiate the console io.

If you don't define it, you will need to provide the proper one when starting the console.

You can also define CONSOLE_RXQ_SIZE and CONSOLE_TXQ_SIZE giving the fifo's buffer size for rx and tx.

The default for these Q size is 1024. Defining them will override the default.

There is also a printf like function available **console_printf**.

This function will write to a local buffer on the stack making it thread safe.

The default size is 80 but you can overrite it by defining CONSOLE_PRINT_SIZE.

## Add and enable usart in CubeMX

Select and enable proper usart in the CubeMX project.

Notice that only enabling a usart may not be enough as default rx/tx pins will be selected and that may
not be the proper one. If that is the case, configure the proper pin as usart rx and tx then enable the
usart with proper baudrate configuration.

When adding new peripherals to a project, files are going to be added to Driver/STM32xxx_HAL_Driver but
may not be added to your project files. If that is the case, add them.

## Starting the console

In the main of the project add the line

```
consoleioInitialize( &CONSOLE_USART, true );

or 

consoleioInitialize( &huart5, false );
```

Put rxenable to false if you do not want interrupts on rx.
