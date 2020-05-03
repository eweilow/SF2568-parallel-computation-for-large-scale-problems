FROM debian

RUN apt-get update && apt-get --fix-missing --yes install openmpi-bin \
  openmpi-common \
  openssh-client \
  openssh-server \
  time

WORKDIR /etc/
COPY . .
ENV OMPI_MCA_btl_vader_single_copy_mechanism=none
RUN mpicc -O3 -o main main.c -lm

WORKDIR /etc/data
CMD mpirun --allow-run-as-root -np 1 /etc/main