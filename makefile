# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato fmoscato@unisa.it
#
# Group:
# Marcone Giuseppe 0622701896 g.marcone2@studenti.unisa.it               
# Pizzulo Rocco Gerardo 0622701990  r.pizzulo@studenti.unisa.it 
# Russo Luigi  0622702071  l.russo86@studenti.unisa.it
#
# This file is part of ParallelTarjan.
#
# ParallelTarjan is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ParallelTarjan is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ParallelTarjan.  If not, see <http://www.gnu.org/licenses/>.



CC=mpicc
GCC=gcc
CFLAGS=-O3 -fopenmp -I./include -lm
GFLAGS=-O3 -o -I./include
LDFLAGS=-lm

all: main main2 sequential test

main: ./src/main.c
	$(CC) $(CFLAGS) -o ./bin/main ./src/main.c $(LDFLAGS)

main2: ./src/main2.c
	$(CC) $(CFLAGS) -o ./bin/main2 ./src/main2.c $(LDFLAGS)

sequential: ./src/sequential.c
	$(GCC) $(GFLAGS) -o ./bin/sequential ./src/sequential.c $(LDFLAGS)

test: ./src/test.c
	$(GCC) $(GFLAGS) -o ./bin/test ./src/test.c $(LDFLAGS)

clean:
	rm -f ./bin/main ./bin/main2 ./bin/sequential ./bin/test
