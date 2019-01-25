//
//  portallookuptable.h
//  enroocraft
//
//  Created by Andrew Farm on 5/23/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef portallookuptable_h
#define portallookuptable_h

#include <map>

#include "portal.h"

class PortalLookupTable {
    std::map<BlockCoord, std::map<BlockCoord, PortalPlane *> *> map;
    
    void add(BlockCoord from, BlockCoord to, PortalPlane *ppp);
    void remove(BlockCoord from, BlockCoord to);
    
public:
    ~PortalLookupTable();
    void addPortalPlane(PortalPlane &pp);
    void removePortalPlane(PortalPlane &pp);
    PortalPlane *getPortalPlane(BlockCoord from, BlockCoord to);
};

#endif /* portallookuptable_h */
