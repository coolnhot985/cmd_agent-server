#!/bin/bash

valgrind --leak-check=full --log-file=valgrind.log -v --error-limit=no ./cmd_agent
