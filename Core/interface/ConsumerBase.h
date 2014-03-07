/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "FilterBase.h"
#include "FilterResult.h"

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/ProcessNodeBase.h"

template<class TTypes>
class Pipeline;

/*
 The base class to implement your own Consumer which run within an Pipeline.
 This class gets the FilterResult, the created products, the settings and the input event ifself
 and can create the output you desire.
 It is not allowed to write to the settings, filter, event and products.
 */

template<class TTypes>
class ConsumerBase: public ProcessNodeBase {
public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;

	virtual ~ConsumerBase() {
	}

	/*
	 * Called before the first Event is passed to this consumer
	 */
	virtual void Init(Pipeline<TTypes> * pipeline) {
		m_pipeline = pipeline;
	}

	/* this method is only called for events which have passed the filter imposed on the
	 * pipeline
	 */
	virtual void ProcessFilteredEvent(event_type const& event,
			product_type const& product) {
	}

	/*
	 *  this method is called for all events
	 *  Depending on the outcome of the pipeline ( a filter might have stopped the producers
	 *  prematurely), some local products might not have been filled
	 */
	virtual void ProcessEvent(event_type const& event,
			product_type const& product,
            FilterResult & result) {
	}

	/*
	 *  this method is called for pipeline which do not operate on event input but on data
	 *  generated by preceeding pipelines
	 */
	virtual void Process() {
	}

	/*
	 * Called after the last event. Overwrite this to store your histograms etc. to disk
	 */
	virtual void Finish() = 0;

	/*
	 * Must return a unique id of the Consumer.
	 */
	virtual std::string GetConsumerId() const = 0;

	/*
	 * Return a reference to the settings used for this consumer
	 */
	setting_type const& GetPipelineSettings() const {
		return this->m_pipeline->GetSettings();
	}

	virtual ProcessNodeType GetProcessNodeType () const
		ARTUS_CPP11_FINAL
		ARTUS_CPP11_OVERRIDE
	{
		return ProcessNodeType::Consumer;
	}

protected:
	Pipeline<TTypes> * m_pipeline;
};
