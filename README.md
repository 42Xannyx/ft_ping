# ft_ping

`ft_ping` is a custom implementation of the ping command, which sends ICMP "echo request" packets to network hosts. This project aims to replicate some of the functionality of the traditional `ping` command available in Unix/Linux systems, with additional educational insights into network programming, compiler attributes, and packet analysis with Wireshark.

## Supported Flags

- `-v`: Verbose output. Provides additional details for each packet response.
- `-f`: Flood ping. Sends packets as fast as they come back or one hundred times per second.
- `-V`: Show version of the ft_ping program.
- `-l`: Preload. Sends that many packets not waiting for reply. Only available for the superuser.
- `-n`: Numeric output only. Does not resolve hostnames to addresses.
- `-w`: Deadline. Specifies how long to wait for a response to the echo request.
- `-W`: Timeout. Time to wait for a response.
- `-p`: Pattern. Allows you to specify up to 16 "pad" bytes to fill out the packet.
- `-r`: Bypass the normal routing tables and send directly to a host on an attached network.
- `-s`: Specifies the number of data bytes to be sent.
- `-T`: Timestamp. Specifies the timestamp option for ICMP requests.
- `--ttl`: Time To Live.
- `--ip-timestamp`: Include IP timestamp in the packet.

## What I Learned

- **Basics of Compiler Attributes**: Gained understanding of how to use compiler attributes for optimizing the code, handling cleanup tasks, and ensuring correct function usage.
- **Logic of Ping**: Explored how ping works under the hood, including sending ICMP packets, handling flags, and interpreting responses.
- **Wireshark**: Learned how to use Wireshark for packet analysis, which helped in understanding network communication and troubleshooting packet issues.

## Compilation and Execution

To compile `ft_ping`, follow these steps:

1. Generate build system files with CMake:
   ```
   cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DDEBUG=OFF
   ```
2. Compile the project with Make:
   ```
   make -C build
   ```
3. Run `ft_ping` (replace `8.8.8.8` with your target IP address or hostname):
   ```
   sudo ./build/ft_ping 8.8.8.8
   ```

**Note**: Superuser privileges (`sudo`) are required for raw socket operations.

### Using Docker

To avoid the need for `sudo` and to make use of Valgrind, you can use Docker:

1. Build your Docker container:
   ```
   docker build -t ft_ping .
   ```
2. Run your Docker container:
   ```
   docker run -it --rm --name=ft_ping_container ft_ping
   ```
3. Inside the Docker container, you can compile and run `ft_ping` without `sudo` and use Valgrind to check for memory leaks.

**Valgrind Check**:

To check for memory leaks with Valgrind inside the Docker container, you can run:

```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/ft_ping 8.8.8.8
```
