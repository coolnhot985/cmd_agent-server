#!/bin/bash

valgrind --tool=memcheck --leak-check=full --log-file=./valgrind.log --show-reachable=yes ./cmd_agent
