FROM ubuntu:18.04

RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y build-essential make gcc openmpi-bin openmpi-common openssh-client openssh-server libopenmpi-dev binutils sudo
RUN useradd -ms /bin/bash mpi
RUN adduser --disabled-password --gecos '' mpiuser
RUN echo "mpiuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
ENV HOME /home/mpirun

RUN mkdir /var/run/sshd
RUN echo 'root:mpiuser' | chpasswd
RUN echo 'mpiuser:mpiuser' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

ENV SSHDIR /home/mpiuser/.ssh/

RUN mkdir -p ${SSHDIR}

ADD ssh/config ${SSHDIR}/config
ADD ssh/id_rsa ${SSHDIR}/id_rsa
ADD ssh/id_rsa.pub ${SSHDIR}/id_rsa.pub
ADD ssh/id_rsa.pub ${SSHDIR}/authorized_keys
ADD hosts /home/mpiuser/hosts
RUN chown -R mpiuser:mpiuser ${SSHDIR}

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
