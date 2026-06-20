#!/bin/bash

# Seeding tests for June 18th
printf "06/18/2026\tAbsolute Match\n" > ~/.calendar
printf "06/18\tYearly Match\n" >> ~/.calendar
printf "18\tDay 18 Match\n" >> ~/.calendar
printf "06 *\tFull Month June Match\n" >> ~/.calendar
printf "06\tDay 06 Match\n" >> ~/.calendar
printf "02 *\tFebruary Wildcard - Should Hide\n" >> ~/.calendar
