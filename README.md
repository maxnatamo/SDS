# SDS

SDS is a dead-simple display server, hoping to be a low-tier alternative to X.Org and/or Wayland.

## Why?

This project is *purely* for my own learning experience. You'd be mad to use this, unironically.

## How?

SDS uses DRM dumb buffers to interact with the framebuffer. This makes the entire process hardware independent.

## Supported hardware

As said above, SDS uses dumb buffers, so almost any GPU should be supported - as long as framebuffers are enabled in the kernel.

Currently, all input devices use the EVDEV input driver.
