<div align="center">

# Genetic Algorithm Tuned Fuzzy Controller

**An embedded fuzzy controller with genetic-algorithm-based centroid tuning.**

![Platform](https://img.shields.io/badge/platform-Arduino%20%2F%20microcontroller-00979D?logo=arduino&logoColor=white)
![Control](https://img.shields.io/badge/control-fuzzy%20logic-6f42c1)
![Optimization](https://img.shields.io/badge/optimization-genetic%20algorithm-0A7F8C)
![Status](https://img.shields.io/badge/status-experimental-orange)

</div>

## Overview

This project combines fuzzy logic control with a genetic algorithm used to tune membership-function centroids. It targets microcontroller-based control applications and was implemented on an experimental bench involving a DC motor.

The setup also includes a servo-driven brake, designed to automate the braking process and make tests and simulations more consistent.

## System concept

The documented project combines the following elements:

- **Fuzzy controller:** maps observed process conditions to a control action.
- **Genetic algorithm:** searches for membership-function centroid configurations.
- **DC motor test bench:** provides the physical system under control.
- **Servo-actuated brake:** supports repeatable braking during experiments.

## Repository contents

```text
.
├── FUZZY_Controller_GA.ino
├── src/
└── README.md
```

## Technology and methods

- Embedded C/C++ / Arduino sketch
- Fuzzy logic
- Genetic algorithms
- DC motor control
- Servo motor actuation

## Getting started

### Requirements

- Arduino-compatible development environment
- Compatible microcontroller
- DC motor and driver suitable for the test setup
- Servo motor and mechanical brake arrangement
- Sensors and wiring matching the implementation

The exact board, pin mapping, sensor model, and wiring must be read from the sketch and source files before connecting hardware.

### Inspect the implementation

Open `FUZZY_Controller_GA.ino` and review the files under `src/`. Confirm the hardware interfaces and safety limits before running the controller on a physical setup.

## Experimental considerations

Controller quality depends on the plant, sensors, sampling, actuator limits, and tuning procedure. The repository describes an experimental implementation; it does not provide a general guarantee of stability or performance on other hardware.

## Project status

Experimental embedded-control project.

## License

No license file was identified in the repository. Contact the author before redistributing or reusing this project.
