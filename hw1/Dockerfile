FROM debian

RUN apt-get update
RUN apt-get --yes install openmpi-bin \
  openmpi-common \
  openssh-client \
  openssh-server

COPY . .
ENV OMPI_MCA_btl_vader_single_copy_mechanism=none
RUN mpicc -o main main.c -lm

RUN mkdir /etc/data
CMD mpirun --allow-run-as-root -np 8 main