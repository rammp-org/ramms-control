# RammsControl

The control-surface model the rest of RAMMS speaks: how a robot **describes**
the controls it offers, and how any input source **drives** them.

```
RammsControl  <-  RammsCore (robot base, contributors)  <-  ramms-controllers
              <-  RammsUI (panels, joysticks)           <-  RammsAccess
              <-  RammsMujocoSupport
```

It is deliberately dependency-light — `Core`, `CoreUObject`, `Engine`,
`EnhancedInput`, and nothing else. No UMG, no physics, no robot. A project can
implement a control surface without pulling in a renderer, and render one
without pulling in a simulator.

## What is in here

| file | what it is |
| --- | --- |
| `RammsControlTypes.h` | `FRammsControlAxis`, `FRammsControlSurface`, `ERammsControlKind / Units / Source` — one control, and everything a generic panel needs to render or drive it |
| `RammsControlSurfaceProvider.h` | `IRammsControlSurfaceProvider` — "here is what I offer", plus a version that changes when the answer does |
| `RammsControlSink.h` | `IRammsControlSink` — "drive this control", with source arbitration |
| `RammsControlIds.h` | the shared Id vocabulary (`drive.forward`, `linkage.<name>.height`, …), so nobody spells them as literals |
| `RammsControlInputMap.h` | `URammsControlInputMap` — Enhanced Input actions bound to control Ids |
| `RammsControlSurfaceRegistry.h` | a world subsystem robots register with, so panels and input components find every controllable robot without naming one |

## Why it is its own plugin

It used to be a module inside the RammsUI plugin. Unreal declares plugin
dependencies per *plugin*, not per module, so every consumer of these types —
RammsCore included — had to take the whole UI plugin, its Slate surface and its
streaming dependency, to get five structs and a handful of interfaces. Nothing
in RammsCore draws a widget.

## Using it

A robot exposes controls by implementing the provider and sink (in RAMMS,
`URammsRobotControlSurfaceComponent` does this and gathers per-component
contributors). Anything that drives a robot — a panel, a keyboard, an autonomy
client — talks to the sink by Id and never names a controller class.
