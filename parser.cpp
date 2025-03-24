#include <iostream>
#include <vector>
#include <set>
#include <cstdint>
#include <fstream>
#include <fcntl.h>
#include <io.h>

constexpr size_t PACKET_SIZE = 188;
constexpr uint8_t SYNC_BYTE = 0x47;

uint16_t extract_pid(const std::vector<uint8_t> &packet)
{
    // PID is 13 bits: last 5 bits of byte 1 and all 8 bits of byte 2
    return ((packet[1] & 0x1F) << 8) | packet[2];
}

bool validate_packet(const std::vector<uint8_t> &packet)
{
    // Check if the packet size is correct
    if (packet.size() != PACKET_SIZE)
    {
        return false;
    }
    // Check if the first byte is the sync byte
    return packet[0] == SYNC_BYTE;
}

bool parse_stream(std::istream &input)
{
    // Store unique PIDs
    std::set<uint16_t> unique_pids;
    // Check if it's the first packet
    bool first_packet = true;
    // Count the number of packets
    size_t packet_count = 0;
    // Count the number of bytes read
    size_t byte_offset = 0;
    // Buffer to store the packet
    std::vector<uint8_t> buffer;
    // Resize the buffer to the packet size
    buffer.resize(PACKET_SIZE);
    // Read the stream
    while (input.read(reinterpret_cast<char *>(buffer.data()), PACKET_SIZE))
    {
        // Get the number of bytes read
        size_t bytes_read = input.gcount();

        // Handle partial first packet
        if (first_packet && bytes_read < PACKET_SIZE)
        {
            return true; // If first packet is not complete, discard it
        }

        if (!validate_packet(buffer))
        {
            std::cerr << "Error: No sync byte present in packet "
                      << packet_count << ", offset " << byte_offset << std::endl;
            return false;
        }

        // Extract the PID
        uint16_t pid = extract_pid(buffer);
        // Insert the PID into the set
        unique_pids.insert(pid);
        // Increment the packet count
        packet_count++;
        // Increment the byte offset
        byte_offset += PACKET_SIZE;
        // Set the first packet flag to false
        first_packet = false;
    }

    // Print unique PIDs in ascending order
    for (uint16_t pid : unique_pids)
    {
        std::cout << "0x" << std::hex << std::uppercase << pid << std::endl;
    }

    return true;
}

int main(int argc, char *argv[])
{
    // Set stdin to binary mode for Windows - Had issues with the input stream otherwise
    #ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    #endif

    if (argc > 1)
    {
        // If filename is provided as argument, read from file
        std::ifstream file(argv[1], std::ios::binary);
        if (!file)
        {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        return parse_stream(file) ? 0 : 1;
    }
    else
    {
        // Otherwise read from stdin
        return parse_stream(std::cin) ? 0 : 1;
    }
}