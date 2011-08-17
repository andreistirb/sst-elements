// Copyright 2010 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2010, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _NODECOMPONENT_H
#define _NODECOMPONENT_H

#include <sst/core/event.h>
#include <sst/core/sst_types.h>
#include <sst/core/component.h>
#include <sst/core/link.h>
#include <sst/core/timeConverter.h>
#include "JobStartEvent.h"
#include "CompletionEvent.h"

class nodeComponent : public SST::Component {
public:

  nodeComponent(SST::ComponentId_t id, SST::Component::Params_t& params);
  int Setup() {return 0;}
  int Finish() {return 0;}

private:
  nodeComponent();  // for serialization only
  nodeComponent(const nodeComponent&); // do not implement
  void operator=(const nodeComponent&); // do not implement

  void handleEvent( SST::Event *ev );
  void handleSelfEvent( SST::Event *ev );

  int jobNum;
  int nodeNum;

  SST::Link* Scheduler;
  SST::Link* SelfLink;

  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_NVP(jobNum);
    ar & BOOST_SERIALIZATION_NVP(nodeNum);
    ar & BOOST_SERIALIZATION_NVP(Scheduler);
    ar & BOOST_SERIALIZATION_NVP(SelfLink);
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int version) 
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_NVP(jobNum);
    ar & BOOST_SERIALIZATION_NVP(nodeNum);
    ar & BOOST_SERIALIZATION_NVP(Scheduler);
    ar & BOOST_SERIALIZATION_NVP(SelfLink);
    //restore links
    Scheduler->setFunctor(new SST::Event::Handler<nodeComponent>(this,
								 &nodeComponent::handleEvent));
    SelfLink->setFunctor(new SST::Event::Handler<nodeComponent>(this,
								&nodeComponent::handleSelfEvent));
  }
    
  BOOST_SERIALIZATION_SPLIT_MEMBER()
 
};

#endif /* _NODECOMPONENT_H */
