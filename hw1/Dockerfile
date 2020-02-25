FROM debian

RUN apt-get update
RUN apt-get --yes install openmpi-bin \
  openmpi-common \
  openssh-client \
  openssh-server

COPY . .
ENV OMPI_MCA_btl_vader_single_copy_mechanism=none
RUN mpicc -O3 -o main main.c -lm

RUN mkdir /etc/data
CMD mpirun --allow-run-as-root -np 8 main whole 10000 -0.25 0.0 0.5 
# CMD \ 
# mpirun --allow-run-as-root -np 8 main whole 1000 -0.25 0.0 0.5 && \
#   mpirun --allow-run-as-root -np 8 main nice_1 1000 -0.9973 -0.2875 100 && \
#   mpirun --allow-run-as-root -np 8 main nice_2 1000 -0.9973 -0.2875 2500 && \
#   mpirun --allow-run-as-root -np 8 main nice_3 1000 -0.9973 -0.2875 15000
# 

#mpirun --allow-run-as-root -np 8 main zoom1_1 2000 -0.9973 -0.2875 0.1000 && \