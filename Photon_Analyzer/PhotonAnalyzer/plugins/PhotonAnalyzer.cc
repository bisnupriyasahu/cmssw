// -*- C++ -*-
//
// Package:    Photon_Analyzer/PhotonAnalyzer
// Class:      PhotonAnalyzer
// 
/**\class PhotonAnalyzer PhotonAnalyzer.cc Photon_Analyzer/PhotonAnalyzer/plugins/PhotonAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Bisnupriya Sahu (U Hyderabad)
//         Created:  Mon, 15 Jul 2019 16:58:11 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


using namespace std;
using namespace edm;


//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PhotonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PhotonAnalyzer(const edm::ParameterSet&);
      ~PhotonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
   

   
   // ----------member data ---------------------------


  Int_t            nPho_;
  vector<float>    phoE_;
  vector<float>    phoPx_;
  vector<float>    phoPy_;
  vector<float>    phoPz_;
  vector<float>    phoEt_;
  vector<float>    phoEta_;
  vector<float>    phoPhi_;

  vector<float>    phoSCE_;
  vector<float>    phoSCRawE_;
  vector<float>    phoSCEta_;
  vector<float>    phoSCPhi_;
  vector<float>    phoSCEtaWidth_;
  vector<float>    phoSCPhiWidth_;


  vector<int>      phohasPixelSeed_;
  vector<int>      phoEleVeto_;
  vector<float>    phoR9_;
  vector<float>    phoR9Full5x5_;
  vector<float>    phoHoverE_;


  vector<float>    phoPFChIso_;
  vector<float>    phoPFChWorstIso_;
  vector<float>    phoPFPhoIso_;
  vector<float>    phoPFNeuIso_;

  edm::EDGetTokenT<edm::View<pat::Photon> >        photonToken_;
  void branchPhotons   (TTree*);
  void Photons_Clear();
  TTree * tree_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PhotonAnalyzer::PhotonAnalyzer(const edm::ParameterSet& iConfig)

{
  photonToken_         = consumes<edm::View<pat::Photon> > (iConfig.getParameter<edm::InputTag>("photonToken"));
   //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_    = fs->make<TTree>("eventTree", "event tree for analysis");
  branchPhotons(tree_);
 
}


PhotonAnalyzer::~PhotonAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------

void PhotonAnalyzer::branchPhotons(TTree* tree){

  tree->Branch("nPho",                     &nPho_);
  tree->Branch("phoE",                     &phoE_);
  tree->Branch("phoPx",                    &phoPx_);
  tree->Branch("phoPy",                    &phoPy_);
  tree->Branch("phoPz",                    &phoPz_);
  tree->Branch("phoEt",                    &phoEt_);
  tree->Branch("phoEta",                   &phoEta_);
  tree->Branch("phoPhi",                   &phoPhi_);


  tree->Branch("phoSCE",                   &phoSCE_);
  tree->Branch("phoSCRawE",                &phoSCRawE_);
  tree->Branch("phoSCEta",                 &phoSCEta_);
  tree->Branch("phoSCPhi",                 &phoSCPhi_);
  tree->Branch("phoSCEtaWidth",            &phoSCEtaWidth_);
  tree->Branch("phoSCPhiWidth",            &phoSCPhiWidth_);

  tree->Branch("phohasPixelSeed",          &phohasPixelSeed_);
  tree->Branch("phoEleVeto",               &phoEleVeto_);
  tree->Branch("phoR9",                    &phoR9_);
  tree->Branch("phoR9Full5x5",             &phoR9Full5x5_);
  tree->Branch("phoHoverE",                &phoHoverE_);


  tree->Branch("phoPFChIso",               &phoPFChIso_);
  tree->Branch("phoPFChWorstIso",          &phoPFChWorstIso_);
  tree->Branch("phoPFPhoIso",              &phoPFPhoIso_);
  tree->Branch("phoPFNeuIso",              &phoPFNeuIso_);

}

void PhotonAnalyzer::Photons_Clear(){

  nPho_ = 0;

  phoE_                        .clear();
  phoPx_                       .clear();
  phoPy_                       .clear();
  phoPz_                       .clear();
  phoEt_                       .clear();
  phoEta_                      .clear();
  phoPhi_                      .clear();


  phoSCE_                      .clear();
  phoSCRawE_                   .clear();
  phoSCEta_                    .clear();
  phoSCPhi_                    .clear();
  phoSCEtaWidth_               .clear();
  phoSCPhiWidth_               .clear();

  phohasPixelSeed_             .clear();
  phoEleVeto_                  .clear();
  phoR9_                       .clear();
  phoR9Full5x5_                .clear();
  phoHoverE_                   .clear();



  phoPFChIso_                  .clear();
  phoPFChWorstIso_             .clear();
  phoPFPhoIso_                 .clear();
  phoPFNeuIso_                 .clear();
}







void
PhotonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  Photons_Clear();
   


   edm::Handle<edm::View<pat::Photon> > photonHandle;
   iEvent.getByToken(photonToken_, photonHandle);

   if (!photonHandle.isValid()) {
     edm::LogWarning("PhotonAnalyzer") << "no pat::Photons in event";
     return;
   }

   for(edm::View<pat::Photon>::const_iterator ipho = photonHandle->begin(); ipho != photonHandle->end(); ++ipho) {

   phoE_                     .push_back(ipho->energy());
   phoPx_                    .push_back(ipho->px());
   phoPy_                    .push_back(ipho->py());
   phoPz_                    .push_back(ipho->pz());
   phoEt_                    .push_back(ipho->et());
   phoEta_                   .push_back(ipho->eta());
   phoPhi_                   .push_back(ipho->phi());

   phoSCE_                   .push_back(ipho->superCluster()->energy());
   phoSCRawE_                .push_back(ipho->superCluster()->rawEnergy());
   phoSCEta_                 .push_back(ipho->superCluster()->eta());
   phoSCPhi_                 .push_back(ipho->superCluster()->phi());
   phoSCEtaWidth_            .push_back(ipho->superCluster()->etaWidth());
   phoSCPhiWidth_            .push_back(ipho->superCluster()->phiWidth());

   phohasPixelSeed_          .push_back((Int_t)ipho->hasPixelSeed());
   phoEleVeto_               .push_back((Int_t)ipho->passElectronVeto());
   phoR9_                    .push_back(ipho->r9());
   phoR9Full5x5_             .push_back(ipho->full5x5_r9());
   phoHoverE_                .push_back(ipho->hadTowOverEm());

   phoPFChIso_               .push_back(ipho->userFloat("phoChargedIsolation"));
   phoPFChWorstIso_          .push_back(ipho->userFloat("phoWorstChargedIsolation"));
   phoPFPhoIso_              .push_back(ipho->userFloat("phoPhotonIsolation"));
   phoPFNeuIso_              .push_back(ipho->userFloat("phoNeutralHadronIsolation"));


   nPho_++;
   
   }

   tree_->Fill();

}





// ------------ method called once each job just before starting event loop  ------------
void 
PhotonAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PhotonAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PhotonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhotonAnalyzer);
