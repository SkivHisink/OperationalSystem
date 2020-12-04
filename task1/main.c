#include "functions.h"

int main(int argc, char* argv[])
{
    int param;
    while ((param = getopt(argc, argv, "ispuU:cC:dvV:")) != -1) {
        switch (param) {
        case 'c':
            print_core_limit();
            break;
        case 'C':
            set_core_limit(optarg);
            break;
        case 'd':
            print_dir();
            break;
        case 'i':
            print_gu_ids();
            break;
        case 'p':
            print_p_ids();
            break;
        case 's':
            make_group_owner();
            break;
        case 'u':
            print_ulimit();
            break;
        case 'U':
            set_ulimit(optarg);
            break;
        case 'v':
            print_env();
            break;
        case 'V':
            put_env(optarg);
            break;
        default: 
            return 1;
        }
    }
    return 0;
}
