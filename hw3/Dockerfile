FROM debian

RUN apt-get update
RUN apt-get --yes install openmpi-bin \
  openmpi-common \
  openssh-client \
  openssh-server

WORKDIR /etc/
COPY . .
ENV OMPI_MCA_btl_vader_single_copy_mechanism=none
RUN mpicc -O3 -o main main.c -lm

WORKDIR /etc/data
CMD mpirun --allow-run-as-root -np 4 /etc/main 1000000