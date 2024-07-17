#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to run a test case
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local timeout_duration="$4"

    echo -e "\nRunning test: ${GREEN}$test_name${NC}"
    echo "Command: $command"
    
    output=$(timeout "$timeout_duration" $command 2>&1)
    exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        echo "Command timed out as expected"
    elif [ $exit_code -ne 0 ]; then
        echo -e "${RED}Command failed with exit code $exit_code${NC}"
    fi

    if [[ "$output" == *"$expected_output"* ]]; then
        echo -e "${GREEN}Test passed${NC}"
    else
        echo -e "${RED}Test failed${NC}"
        echo "Expected output to contain: $expected_output"
        echo "Actual output:"
        echo "$output"
    fi
}

# Test cases

# Test -? flag
run_test "Help flag (-?)" "./ft_ping -?" "Usage:" "5s"

# Test -v flag
run_test "Verbose flag (-v)" "./ft_ping -v 8.8.8.8" "PING 8.8.8.8" "5s"

# Test -V flag
run_test "Version flag (-V)" "./ft_ping -V" "ft_ping from 42Xannyx 20240401" "5s"

# Test -w flag
run_test "Timeout flag (-w)" "./ft_ping -w 5 8.8.8.8" "PING 8.8.8.8" "10s"

# Test with IPv4 address
run_test "IPv4 address" "./ft_ping 8.8.8.8" "" "5s"

# Test with invalid address
run_test "Invalid address" "./ft_ping invalidaddress" "getaddrinfo: Temporary failure in name resolution" "5s"

# Test with multiple flags
run_test "Multiple flags" "./ft_ping -v -w 3 8.8.8.8" "PING 8.8.8.8" "10s"

# Test with non-existent flag
run_test "Non-existent flag" "./ft_ping -z 8.8.8.8" "invalid option" "5s"

# Comparison with system ping (if available)
if command -v ping &> /dev/null; then
    echo -e "\n${GREEN}Comparing with system ping${NC}"
    echo "ft_ping output:"
    timeout 5s ./ft_ping 8.8.8.8 
    echo "System ping output:"
    timeout 5s ping 8.8.8.8 
else
    echo -e "\n${RED}System ping not available for comparison${NC}"
fi

echo -e "\n${GREEN}All tests completed${NC}"
