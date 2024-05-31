#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

int main()
{
  char output[1035];

  /* Open the command for reading. */

  std::cout << "\nGetting zombie processes...\n\n";
  FILE *fp = popen("/bin/ps aux | awk '$8 ~ /^[Zz]/'", "r"); // Run ps command and take note of all processes with Zombie state

  if (fp == NULL) {
    std::cout << "Failed to read processes\n";
    exit(1);
  }

  // Read 
  std::vector<std::string> childPID{};

  std::cout << "The following processes are in the Zombie state:\n";
  while (fgets(output, sizeof(output), fp) != NULL) {
    std::string temp = output;
    std::cout << temp << "\n";
    std::stringstream ss(temp);
    std::string word;

    int i = 0;
    while (ss >> word) {
      switch (i) {
      case 1:{
	childPID.push_back(word);
      }break;
      }
      i++;
    }
  }
  
  if (childPID.size() == 0) {
    std::cout << "\033[1;31mThere are currently no Zombie processes.\033[0m\n\nExiting program...\n\n";
    return 0;
  }

  char cleanup;
  std::cout << "\n? Do you want to clean up all Zombie tasks? (Y/n) \033[1;36m";
  std::cin >> cleanup;
  std::cout << "\033[0m";

  if (cleanup == 'y' || cleanup == 'Y') {

    std::cout << "\nDestroying all Zombie processes...\n";
    for (std::string child : childPID){
      
      std::string command = "/bin/ps -o ppid= -p ";
      command.append(child);
      fp = popen(command.c_str(), "r"); // Run ps command and take note of all processes with Zombie state

      if (fp == NULL) {
	std::cout << "Failed to read processes\n";
	exit(1);
      }
      fgets(output, sizeof(output), fp);

      std::string temp = "kill -9 ";
      temp.append(output);
      system(temp.c_str());
    }
  }
  else {
    std::cout << "\nCancelling operation. Terminating program\n\n";
  }

  free(fp);

  return 0;
}
