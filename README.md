# Data Logger and Sensor Package with ESP32C6, ESP-NOW

**Data Logger, ESP-NOW, DataViz Webserver, LSM330DHC**

Future Work: Power Management, Data handling for 500Hz+ logging, GPS Integration, "Fun" metrics

## Table of Contents

 - [Introduction](#introduction)
 - [Schematics](#schematics)

## Introduction
// Write a short sentence or two about this project and what it does. Be sure to include a link and a screenshot (we're front end devs so we can actually see our work!).

An open sourced data logger to determine the acceleration data at the axle of a bike, primarily used for ride optimization for mountain biking, however could be the basis for any sort of data acquisition system. The initial idea and form factor for this system came from disbelief of the Fox "Live Valve NEO" system, and an initial mis-understanding of how the NEO system performed. (Below image is LiveValve Neo, NOT MY DATA LOGGER)

![NeoOverview.jpg](assets/NeoOverview.jpg)

In the world of mountain biking (or any wheeled, speed based sport), the optimization of forces at the contact patch between the ground and your wheel has been the golden rule amongst all improvements, and controlling these forces yada yada yada I want to better understand the acceleration of my front and rear wheel in order to improve (or more likely pretend to improve) the setup of the front and rear shocks on my bike. Hopefully this will lead to better handling characteristics of the bike, but at the very least will be a fun project to dip my toe into coding, electronics, data analysis, and mountain biking. 

Generally, shock potentiometers are used to track shock position as they go through their travel and then is turned into velocity for analysis, however these are expensive sensors to use to get started with electronics. So instead, this system will take accelerometer data at the axle of the front and rear wheel to determine the shock velocities, as well as providing some additional information such as steer or lean angle that could be used in the future. This accelerometer data will be sent wirelessly to a central data logger, where the information will be stored on an SD card, along with being hosted on a webserver that can be accessed from another device on the same network, such as a phone or laptop. 

![OverviewBlockDiagram.jpg](assets/OverviewBlockDiagram.jpg)

For a little bit more detail, the peripheral sensor nodes (naming convention still in progress) will be mounted to the front and rear brake caliper mounts, and will be based on Seeed Studio XIAO ESP32C6 boards wired through SPI to Adafruit LSM330DHC accelerometers, powered by a small rechargeable	 LiPo battery. These peripheral sensors will communicate to the central node (again, naming is WIP) through ESP-NOW, a wifi based communication protocol specifically between ESP chips. The ESP32C6 board has an integrated antenna along with a built-in power management chip, reducing the overall effort to get to a Minimum Viable Product (MVP), or at the very least something to play with. 

The central node will be wired through SPI to an SD card reader to store all of the logged data. This central node will also handle any data analysis shown on the hosted webserver. 

## Schematics

**Tech used:** HTML, CSS, JavaScript, Framework of choice

Here's where you can go to town on how you actually built this thing. Write as much as you can here, it's totally fine if it's not too much just make sure you write *something*. If you don't have too much experience on your resume working on the front end that's totally fine. This is where you can really show off your passion and make up for that ten fold.

## Optimizations
*(optional)*

You don't have to include this section but interviewers *love* that you can not only deliver a final product that looks great but also functions efficiently. Did you write something then refactor it later and the result was 5x faster than the original implementation? Did you cache your assets? Things that you write in this section are **GREAT** to bring up in interviews and you can use this section as reference when studying for technical interviews!

## Lessons Learned:

No matter what your experience level, being an engineer means continuously learning. Every time you build something you always have those *whoa this is awesome* or *wow I actually did it!* moments. This is where you should share those moments! Recruiters and interviewers love to see that you're self-aware and passionate about growing.



