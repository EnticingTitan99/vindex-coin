# Multistage docker build, requires docker 17.05

# builder stage
FROM ubuntu:26.04 AS builder

RUN set -ex && \
    apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get --no-install-recommends --yes install \
        build-essential \
        ca-certificates \
        cmake \
        curl \
        git \
        pkg-config

WORKDIR /src
COPY . .

ARG NPROC
RUN set -ex && \
    git submodule init && git submodule update && \
    rm -rf build && \
    if [ -z "$NPROC" ] ; \
    then make -j$(nproc) depends target=x86_64-linux-gnu ; \
    else make -j$NPROC depends target=x86_64-linux-gnu ; \
    fi

# runtime stage
FROM ubuntu:26.04

ENV UID=999

RUN set -ex && \
    apt-get update && \
    apt-get --no-install-recommends --yes install ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt
COPY --from=builder /src/build/x86_64-linux-gnu/release/bin /usr/local/bin/

# Create vindex user
RUN useradd --system --user-group --uid $UID vindex && \
	mkdir -p /wallet /home/vindex/.bitvindex && \
	chown -R vindex:vindex /home/vindex/.bitvindex && \
	chown -R vindex:vindex /wallet

# Contains the blockchain
VOLUME /home/vindex/.bitvindex

# Generate your wallet via accessing the container and run:
# cd /wallet
# vindex-wallet-cli
VOLUME /wallet

EXPOSE 18080
EXPOSE 18081

# switch to user vindex
USER vindex

ENTRYPOINT ["vindexd"]
CMD ["--p2p-bind-ip=0.0.0.0", "--p2p-bind-port=18080", "--rpc-bind-ip=0.0.0.0", "--rpc-bind-port=18081", "--non-interactive", "--confirm-external-bind"]
