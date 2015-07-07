
#pragma once

//#include <boost/scoped_ptr.hpp>

#include <TH1.h>
#include "TROOT.h"

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Utility/interface/RootFileHelper.h"

#include <TTree.h>

#include "Artus/Core/interface/ConsumerBase.h"
#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/DefaultValues.h"
#include "Artus/Configuration/interface/ArtusConfig.h"


template < class TTypes >
class RunTimeConsumer: public ConsumerBase<TTypes>{
public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;

	virtual std::string GetConsumerId() const
		ARTUS_CPP11_OVERRIDE
	{
		return "RunTimeConsumer";
	}

	RunTimeConsumer():
		ConsumerBase<TTypes>()
	{
	}

	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE
	{
		ConsumerBase<TTypes>::Init(settings);
		std::vector<std::string> processors = settings.GetAllProcessors();

		// save file
		RootFileHelper::SafeCd(settings.GetRootOutFile(),
				settings.GetRootFileFolder());

		m_tree = new TTree("runTime", "runTime");

		// initialize branches
		int i=0;
		std::string processorName;
		m_runTime.resize(processors.size());
		for (std::string processor : processors) {
		    processorName = ArtusConfig::ParseProcessNode(processor).second;
		    m_processorNames.push_back(processorName);
			m_tree->Branch(processorName.c_str(), &(m_runTime[i]), (processorName + "/I").c_str());
			i++;
		}
	}

	virtual void Finish(setting_type const& setting) ARTUS_CPP11_OVERRIDE
	{
		// save file
		RootFileHelper::SafeCd(setting.GetRootOutFile(),
				setting.GetRootFileFolder());

		m_tree->Write("runTime");
	}

	virtual void ProcessEvent(event_type const& event,
	                          product_type const& product,
	                          setting_type const& setting,
	                          FilterResult & filterResult)
	{
		ConsumerBase<TTypes>::ProcessEvent(event, product, setting, filterResult);

		// set values of runtime
		int i=0;
		for (std::string processor : m_processorNames) {
			m_runTime[i]=SafeMap::GetWithDefault (product.processorRunTime, processor, DefaultValues::UndefinedInt);
			i++;
		}

		// fill tree
		m_tree->Fill();
	}

protected:
	std::vector<std::string> m_processorNames;
	std::vector<int> m_runTime;
	TTree* m_tree = 0;

};
