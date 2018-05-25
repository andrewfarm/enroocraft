//
//  portallookuptable.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/23/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "portallookuptable.h"

void PortalLookupTable::add(BlockCoord from, BlockCoord to, PortalPlane *ppp) {
    const auto mapEntry = map.find(from);
    
    std::map<BlockCoord, PortalPlane *> *toBlockMap;
    if (mapEntry == map.end()) {
        toBlockMap = new std::map<BlockCoord, PortalPlane *>();
    } else {
        toBlockMap = mapEntry->second;
    }
    
    toBlockMap->insert(std::make_pair(to, ppp));
    map.insert(std::make_pair(from, toBlockMap));
}

void PortalLookupTable::remove(BlockCoord from, BlockCoord to) {
    const auto mapEntry = map.find(from);
    
    if (mapEntry != map.end()) {
        std::map<BlockCoord, PortalPlane *> *toBlockMap = mapEntry->second;
        toBlockMap->erase(to);
        
        if (toBlockMap->size() == 0) {
            map.erase(mapEntry);
            delete toBlockMap;
        }
    }
}

PortalLookupTable::~PortalLookupTable() {
    for (const auto &mapEntry : map) {
        delete mapEntry.second;
    }
}

void PortalLookupTable::addPortalPlane(PortalPlane &pp) {
    for (const auto &between : pp.betweenBlocks) {
        add(between.first, between.second, &pp);
        add(between.second, between.first, &pp);
    }
}

void PortalLookupTable::removePortalPlane(PortalPlane &pp) {
    for (const auto &between : pp.betweenBlocks) {
        remove(between.first, between.second);
        remove(between.second, between.first);
    }
}

PortalPlane *PortalLookupTable::getPortalPlane(BlockCoord from, BlockCoord to) {
    const auto mapEntry = map.find(from);
    if (mapEntry == map.end()) {
        return nullptr;
    }
    std::map<BlockCoord, PortalPlane *> *toBlockMap = mapEntry->second;
    const auto toBlockMapEntry = toBlockMap->find(to);
    if (toBlockMapEntry == toBlockMap->end()) {
        return nullptr;
    }
    return toBlockMapEntry->second;
}
