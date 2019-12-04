#ifndef VPN_BUILDER_ABS
#define VPN_BUILDER_ABS

#include <memory>
#include <vector>


#include "IPCommon.h"
#include "TapAbs.h"
#include "TapCommon.h"


namespace vpn {
    

class BuilderAbs
{

public:
    BuilderAbs() {}
    virtual ~BuilderAbs() {}

protected:
    bool load_dns_from_conf();
    bool prepare_tap();


    std::vector <DNS<IPVersion::V4>> dns;
    std::vector <Route<IPVersion::V4>> route;
    Tap tap;
private:
   
};





} // namespace vpn



#endif // VPN_BUILDER_ABS
