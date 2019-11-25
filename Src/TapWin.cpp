#ifdef _WIN32
// Windows only !

#include "TapWin.h"
#include "Logger.h"


#include <iphlpapi.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

namespace vpn {

Tap::~Tap() {
    if (ip_forward_table != nullptr) {
        Logger::getInstance().info("~Tap: Not-null ip_forward_table when destructed");
    }
}


bool Tap::store_route_table() {
    this -> ip_forward_table = this -> get_route_table();
    if (ip_forward_table == nullptr) {
        return false;
    }
    return true;
}


bool Tap::clear_route_table() const {
    MIB_IPFORWARDTABLE * current_table;
    if ((current_table = get_route_table()) == nullptr) {
        return false;
    }
    // assume no issues
    bool status = true;
    // delete all rows from route table
    for (DWORD i=0; i < current_table -> dwNumEntries; ++i) {
        if (DeleteIpForwardEntry(&(current_table->table[i])) != ERROR_SUCCESS) {
            Logger::getInstance().info("Tap::clear_route_table: cannot remove entery");
            status = false;
        }
    }
    FREE(current_table);
    return status;
}


bool Tap::restore_route_table() {
    // assume no issues
    bool status = true;
    // clear overriten table
    this -> clear_route_table();
    // insert old rows
    for (DWORD i = 0; i < ip_forward_table -> dwNumEntries; i++) {
        if (SetIpForwardEntry(&(ip_forward_table -> table[i])) == NO_ERROR) {
            Logger::getInstance().info("Tap::restore_route_table: cannot insert");
            status = false;
        }
    }
    FREE(ip_forward_table);
    ip_forward_table = nullptr;
    return status;
}


MIB_IPFORWARDTABLE * Tap::get_route_table() const {
    MIB_IPFORWARDTABLE * table;
    DWORD size = 0;

    table = (MIB_IPFORWARDTABLE *) MALLOC(sizeof (MIB_IPFORWARDTABLE));
    if (table == NULL) {
        Logger::getInstance().error("Tap::save_route_table: Error allocating memory");
        return nullptr;
    }

    if (GetIpForwardTable(table, &size, 0) == ERROR_INSUFFICIENT_BUFFER) {
        // we need more alocated memory !
        // release old table
        FREE(table);
        table = (MIB_IPFORWARDTABLE *) MALLOC(size);
        if (table == nullptr) {
            Logger::getInstance().error("Tap::save_route_table: Error allocating memory");
            return nullptr;
        }
    }
    // Note: GetIpForwardTable entries are in network byte order 
    if (GetIpForwardTable(table, &size, 0) != NO_ERROR) {
        Logger::getInstance().error("Tap::save_route_table: GetIpForwardTable failed");
        return nullptr;
    }
    return table;
}




} // namespace vpn

#endif // _WIN32
