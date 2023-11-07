#  TyOS - Tiny Operating System

##  Overview
<p>TyOS is a really (really!) simple operating system prototype demonstrating
 the two stage loading.
</p>
<p>First stage, the boot loader, fits entirely within
 the 512-byte master boot record of a USB stick. It is meant to be loaded
 through legacy BIOS boot method and execute in real mode on any x86 platform.
</p>
<p>Upon loading, stage1 loads the second stage right after the boot loader.
 The second stage does nothig, except by printing a welcome message.
 Everyting runs in x86 real-mode (we're not going 32bit yet).
</p>
