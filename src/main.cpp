#include <iostream>

#include <sockpp/tcp_acceptor.h>

//#include <stdio.h>

void panic(const char* string) {
    printf("Program panicked: \"%s\"\n", string);
    fflush(stdout);

    std::exit(EXIT_FAILURE);
}

void panic(const std::string& string) {
    panic(string.c_str());
}

struct Position3I {
    int x, y, z;
};

Position3I read_pos(unsigned char*& input) {
    Position3I result;

    result.x = static_cast<std::int32_t>(*input++) >> 38;
    result.y = static_cast<std::int32_t>(*input++) & 0xFFF;
    result.z = (static_cast<std::int32_t>(*input++) << 26 >> 38);

    return result;
}

std::pair<std::int32_t, std::uint32_t> read_varint(unsigned char*& input) {
    std::uint_fast32_t num_read = 0;
    std::int32_t result = 0;
    unsigned char read;
    do {
        read = *input++;
        std::uint32_t value = (read & 0b01111111);
        result |= (value << (7 * num_read));

        num_read++;
        if (num_read > 5) {
            panic("VarInt is too big");
        }
    } while ((read & 0b10000000) != 0);

    return std::make_pair(result, num_read);
}

std::string read_str(unsigned char*& input) {
    auto length = read_varint(input).first;
    std::string str(reinterpret_cast<char*>(input), length);
    input += length;
    return str;
}

std::uint16_t read_ushort(unsigned char*& input) {
    std::uint16_t result = 0u;
    result |= static_cast<std::uint16_t>(*input++) << 8;
    result |= static_cast<std::uint16_t>(*input++);
    return result;
}

void handle_connection(sockpp::tcp_socket socket) {
    printf("Connection accepted!\n");

    unsigned char buf[1024];
    size_t bytes_read = socket.read(buf, sizeof buf);
    printf("Received %d bytes\n", bytes_read);

    unsigned char* ptr = buf;

    auto length = read_varint(ptr).first;
    auto [packet_id, packet_id_bytes] = read_varint(ptr);
    length -= packet_id_bytes;

    auto protocol_ver = read_varint(ptr).first;
    auto server_address = read_str(ptr);
    auto port = read_ushort(ptr);
    auto next_state = read_varint(ptr).first;

    printf("Handshake {\n\tProtocol version:\t%d\n\tServer address:\t\t\"%s\"\n\tPort:\t\t\t%d\n\tNext state:\t\t%d\n}\n", protocol_ver, server_address.c_str(), port, next_state);
}

int main() {
    //std::cout << "Welcome to Basalt!" << std::endl;

    sockpp::socket_initializer sockInit;

    int16_t port = 12345;
    sockpp::tcp_acceptor acc(port);

    if (!acc) {
        std::cout << acc.last_error_str();
    }

    printf("Started listening..\n");


    while (true) {
        // Accept a new client connection
        sockpp::tcp_socket sock = acc.accept();

        if (!sock) {
            std::cerr << "Error accepting incoming connection: "
                      << acc.last_error_str() << std::endl;
        }
        else {
            handle_connection(std::move(sock));

            // Create a thread and transfer the new stream to it.
            //thread thr(run_echo, std::move(sock));
            //thr.detach();
        }
    }

    return 0;
}
