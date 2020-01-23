FROM debian

RUN apt-get update
RUN apt-get --yes install openmpi-bin \
  openmpi-common \
  openssh-client \
  openssh-server

COPY . .
RUN mpicc -o main main.c -lm

CMD mpirun --allow-run-as-root -np 4 main