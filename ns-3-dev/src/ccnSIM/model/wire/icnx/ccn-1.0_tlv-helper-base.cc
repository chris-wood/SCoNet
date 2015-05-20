/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014,2015 University of California, Santa Cruz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: James Mathewson <jlmathew@soe.ucsc.edu>
 *
 * 
 */
#include "ccn-1.0_tlv-helper-base.h"


namespace wire {
namespace icnx {

class TLV_TLVDATA;

namespace tlvUtils
{
  //should be in a seperate file
  

  TlvHelperBase::TlvHelperBase() {}
  TlvHelperBase::~TlvHelperBase() {}
  void
  TlvHelperBase::Configure(const std::string &fileName)
  {
       pugi::xml_parse_result result = doc.load_file (fileName.c_str());

      if (!result) {
         std::cout << "TlvHelper failed to find file:" << fileName << " with errors " << result.description() << std::endl;
         assert(0);
      }

      pugi::xml_node tlvFormat = doc.child ("TLVFORMAT");

      RecursiveConfigure(tlvFormat, TlvRange(UNDEF_TLV, UNDEF_TLV) );
      TrackTlvOrder();

  }
  
 void
  TlvHelperBase::RecursiveConfigure(pugi::xml_node &tlvParentFormat, const TlvRange &parentRange)
  {
   
      //get info

//static std::string a="";
//std::cout << a << tlvParentFormat.attribute("name").value() << " id=" << tlvParentFormat.attribute("id").as_int() << " upperRange=" << tlvParentFormat.attribute("endRangeId").as_int() << " type=" << tlvParentFormat.attribute("type").value() << std::endl;

//a.append(" ");
      ConfigData currentObject;
      currentObject.parentId = parentRange;
      currentObject.name = tlvParentFormat.attribute("name").value();
      std::string temp= tlvParentFormat.attribute("type").value();
      tlvTypeId rightRange =  static_cast<tlvTypeId>(tlvParentFormat.attribute("endRangeId").as_int()) ;
      tlvTypeId leftRange = static_cast<tlvTypeId>(tlvParentFormat.attribute("id").as_int());

      TlvRange myRange(leftRange, rightRange);
      if (temp.compare(0,3, "opt")) { //not optional
        currentObject.isOptional = false;
      } else { //optional
        currentObject.isOptional = true;
      }
      if (temp.compare(temp.size()-5,5,"Value")) {
        currentObject.isContainer = true;
      } else {
        currentObject.isContainer = false;
      }


      //add it if it doesnt exist before
      if (! nameToIdMap.count(currentObject.name)) {
        nameToIdMap[currentObject.name] = myRange;   //name to tlvId
        currentObject.idRange = myRange;
        for (int i=static_cast<int>(leftRange); i <=static_cast<int>(rightRange); i++) 
        {
          configMap[static_cast<tlvTypeId>(i)]=currentObject;
        }
      }

//ConfigData check = (configMap.find(leftRange))->second;
//ConfigData check = configMap[leftRange];
//std::cout << a << check.name << "[" << myRange << "] with parent of (" << check.parentId << ") and container:" << check.isContainer << " and Optional:" << check.isOptional << std::endl;

for (pugi::xml_node tlvFormat = tlvParentFormat.first_child(); tlvFormat; tlvFormat = tlvFormat.next_sibling())
      {
         RecursiveConfigure(tlvFormat, myRange );
//a.erase(a.length()-1, 1);
      }


  }
  
   //create ONLY if it doesnt exist, must all be container types
 //return TLV pointing to tlvType
   TLV_TLVDATA *  
    TlvHelperBase::SetParentTlvs(TLV_TLVDATA *baseTlv,tlvTypeId tlvType, bool createTlv) const
    {
       //get correct tlv range
    if (UNDEF_TLV == tlvType)
      return NULL;

    if (NULL == baseTlv)
      return NULL;

      ConfigData tlvInfo = configMap.find(tlvType)->second;
      TlvRange tlvRange = static_cast<TlvRange>(tlvInfo.idRange);

      std::list<TlvRange> tlvHeirarchyList = tlvHeirarchyMap.find(tlvRange)->second;
     std::list<TlvRange>::reverse_iterator rit;

/*     std::cout << tlvHeirarchyList.size() << "elements in list " << std::endl;
for (rit = tlvHeirarchyList.rbegin(); rit !=     tlvHeirarchyList.rend(); rit++)
     {
      
       ConfigData tlv = configMap.find(rit->getLowerBound())->second;
       TlvRange range = tlv.idRange;
std::cout << "identify " << tlv.name << "(" << range << ")" << std::endl;
     }
*/

     TLV_TLVDATA *currentTlv = baseTlv;
     //tlvTypeId currentType = rit->getLowerBound(); 

      /* if ((NULL == currentTlv) || (tlvHeirarchyList.empty()) )
       {
         //add tlv
         if (!createTlv) {
           return NULL;
         }
         std::list<TlvRange>::reverse_iterator rParent = tlvHeirarchyList.rbegin();
         tlvTypeId rootId = rParent->getLowerBound();
            
         currentTlv = dynamic_cast<TLV_TLVDATA *>(CreateTlvFromType(rootId, true));  //FIXME TODO should be interest or data, like rend()
std::cout << "root was null, now its" << std::endl;
currentTlv->XmlPrint(std::cout);
       }
       */
rit = tlvHeirarchyList.rbegin();
ConfigData tlv = configMap.find(rit->getLowerBound())->second;
TlvRange range = tlv.idRange;
rit++;
     for (; rit != tlvHeirarchyList.rend(); rit++)
     {
//currentTlv->XmlPrint(std::cout);

       ConfigData tlvChild = configMap.find(rit->getLowerBound())->second;
       TlvRange rangeChild = tlv.idRange;
//std::cout << "checking " << tlv.name << "(" << range << ")" << std::endl;
//std::cout << "   child " << tlvChild.name << "(" << rangeChild << ")" << std::endl;
       if (!tlv.isContainer) {
           throw std::invalid_argument("Tlv parent is not a container!");
       }
       //correct parent tlv?
       //tlvTypeId currentType = static_cast< tlvTypeId >(currentTlv->GetType());
      // if (!range.isInRange(currentType))
       //{
       //    throw std::invalid_argument("Invalid tlv heirarchy!");
       //}

       //if not, we create it, add it to parent tlv, and continue.
      
       if (!currentTlv->ExistTlvData(rit->getLowerBound(), rit->getUpperBound())) {
//  std::cout << rit->getLowerBound() << " does not exist" << std::endl;         
         if (!createTlv)
         {
           return NULL;
         }
         TLV_TLVDATA *childTlv = dynamic_cast<TLV_TLVDATA *>(CreateTlvFromType(rit->getLowerBound()));
         currentTlv->SetData(childTlv);
         currentTlv = childTlv;
       } 
       else { //std::cout << "does exist" << std::endl; 
          currentTlv = dynamic_cast<TLV_TLVDATA *>(currentTlv->GetFirstTlvData(rit->getLowerBound(), rit->getUpperBound()));
       }
       tlv = tlvChild;
       range = rangeChild;
     }
     return currentTlv;

    }

   TLV_BASE *
     TlvHelperBase::CreateValidTlv() const
     {

       //Get root TLV
       std::map<tlvTypeId, ConfigData>::const_iterator it = configMap.begin();
       ConfigData config = (*it).second;
       tlvTypeId undef = UNDEF_TLV;
       while (!config.parentId.isInRange(undef)) {  //not at root, try again
          it++;
          config = (*it).second;
       }
       TlvRange range = config.idRange;
       tlvTypeId type = range.getLowerBound();
       TLV_TLVDATA *parentTlv = dynamic_cast<TLV_TLVDATA *>(CreateTlvFromType(type));
       //it++;  //this would be wrong ...
       for (it = configMap.begin(); it != configMap.end(); it++)
       {
         config = (*it).second;
         range = config.idRange;
         type = range.getLowerBound();
         bool test = ExistTlv(parentTlv, range);
//std::cout << "Create valid tlv range " << range << " already " << test << std::endl;
         if (!(test)) { //dont instantiate if it already exists
           //only instantiate required components
           if (!config.isOptional) {
//std::cout << "create type:" << type << std::endl;
             TLV_BASE *newTlv = CreateTlvFromType(type);
             SetTlv(parentTlv, newTlv);
           }
         }

       }

       //Go through list of non optional end points, create each one


       //return 
       return parentTlv;

     }

 //Set replaces IF it exists, others adds it
  void   
    TlvHelperBase::SetTlv(TLV_BASE *rootTlv, TLV_BASE *insertTlv, bool deleteTlv) const
    {
       TLV_TLVDATA *baseTlv = dynamic_cast<TLV_TLVDATA *>(rootTlv);

       //add parent if it doesnt exist
       tlvTypeId tlvType = static_cast<tlvTypeId>(insertTlv->GetType());
       std::map<tlvTypeId, ConfigData>::const_iterator it;
       it = configMap.find(tlvType);
//std::cout << "name is " << (it->second).idRange << std::endl;
       ConfigData data = it->second;
       //tlvTypeId parentType = static_cast<tlvTypeId>(data.parentId.getLowerBoound());
       tlvTypeId parentType = data.parentId.getLowerBound();
//std::cout << "Before parent append:" << std::endl;
//baseTlv->XmlPrint(std::cout);
       //JLM added 4-3-15
       TLV_TLVDATA *parentTlv;
       if (UNDEF_TLV == parentType) {
          parentTlv = baseTlv;
       } else {

          parentTlv = SetParentTlvs(baseTlv, parentType);
       }

//std::cout << "\nAfter parent append:" << std::endl;
//baseTlv->XmlPrint(std::cout);
      //delete old entry if it exists, then add myself
       if (deleteTlv)
       {
         //faster to delete from the parent, instead of researching entire tlv
         //parentTlv->DeleteTlvData(parentType, data.parentId.getUpperBound());
         parentTlv->DeleteTlvData(data.idRange.getLowerBound(), data.idRange.getUpperBound());
       }
       parentTlv->AppendTlvData(insertTlv);
//std::cout << "\nAfter  append:" << std::endl;
//baseTlv->XmlPrint(std::cout);
    }
  //Append can have multiple tlvs of the same type
  void
  TlvHelperBase::AppendTlv(TLV_BASE *baseTlv, TLV_BASE *appendTlv) const
  {
      SetTlv(baseTlv, appendTlv, false);
  }

  //Does it exist?
  bool
  TlvHelperBase::ExistTlv(TLV_BASE* tlv, const std::string &name) const
  {
    std::map < std::string, TlvRange >::const_iterator it =  nameToIdMap.find(name);
    TlvRange tlvRange = it->second;
    
    return ExistTlv(tlv, tlvRange );
  }
  
  bool
  TlvHelperBase::ExistTlv(TLV_BASE* tlv, tlvTypeId tlvId) const
  {
    TlvRange id(tlvId, tlvId);
    return ExistTlv(tlv, id);
  }

  bool
  TlvHelperBase::ExistTlv(TLV_BASE* tlv, TlvRange &tlvId) const
  {
    if (NULL == tlv)
      return false;
    tlvTypeId undef = UNDEF_TLV;
    if (tlvId.isInRange(undef))
      return false;

    tlvTypeId type = static_cast< tlvTypeId > (tlv->GetType());
    if (tlvId.isInRange(type))
    {
      return true;
    }

       std::map<tlvTypeId, ConfigData>::const_iterator it;
       it = configMap.find(tlvId.getLowerBound());
       ConfigData data = it->second;
       TlvRange parentRange = data.parentId;
       tlvTypeId parentType = parentRange.getLowerBound();
    if (parentRange.isInRange(undef)) { //the TLV must be the type wanted
        return false;
    }
//std::cout << "testing " << tlvId << " --- ";
     TLV_TLVDATA *parentTlv = SetParentTlvs(dynamic_cast<TLV_TLVDATA *>(tlv), parentType, false);
     if (parentTlv) {
         //return (NULL != parentTlv->GetFirstTlvData(tlvId.getLowerBound(), tlvId.getUpperBound()));
         TlvTypeRange tlvRange;
         tlvRange.setUpperBound(tlvId.getUpperBound() );
         tlvRange.setLowerBound(tlvId.getLowerBound() );
         return parentTlv->ExistTlvData(tlvRange);
     } else {
       return false;
     }
      
  }

  ::wire::icnx::TLV_TYPE_t
  TlvHelperBase::GetId(const std::string &tlvName) const
  {
     TlvRange rng = (nameToIdMap.find(tlvName))->second;
     return static_cast< ::wire::icnx::TLV_TYPE_t >(rng.getLowerBound());
     //TODO need a get range for name too
  }

  void
  TlvHelperBase::TrackTlvOrder()
  {
      //create a list of correct order     
     //get a list of all TLV's
     std::list< TlvRange > tlvList; 
     for (std::map<tlvTypeId, ConfigData >::const_iterator it=configMap.begin(); it!=configMap.end(); ++it)
     {
       tlvList.push_back((it->second).idRange);
     }
     tlvList.unique();

//std::cout << std::endl << "there are " << tlvList.size() << " TLV's defined" << std::endl;
     TlvRange ParentRange(UNDEF_TLV,UNDEF_TLV);
     while (!tlvList.empty())
     {
         TlvRange idOrig = tlvList.front();
         tlvList.pop_front();
         TlvRange id = idOrig;
         ConfigData data;
         std::list< TlvRange > heirarchyList;
         do {
//std::cout << id << "->";
            heirarchyList.push_back(id);
            data = configMap[id.getLowerBound()];
            id = data.parentId;         
         } while (id != ParentRange );
         tlvHeirarchyMap[ idOrig ] = heirarchyList;
//std::cout << std::endl;
     }


  }

  //Delete tlv
  void
  TlvHelperBase::DeleteTlv(TLV_BASE *tlv,  tlvTypeId tlvId) const
  {
    TlvRange tlvRange(tlvId, tlvId);
    DeleteTlv(tlv, tlvRange);
  }
  //JLM FIXME TODO need to modify tlv if its an interest/data tlv.
  //we cant modify the original passed pointer yet
  //for now we must assume tlv is NOT a root tlv
  //also delete should delete the specified range, so we dont want to settle
  //for getting first tlv in range match
  void
  TlvHelperBase::DeleteTlv(TLV_BASE *tlv, TlvRange &tlvRange) const
  {
    //Delete is a slight problem, as you must delete the parents, if
    //they are optional
    //messy, dont clean up optional empty parents .. yet JLM FIXME TODO

    ConfigData data = (configMap.find(tlvRange.getLowerBound()))->second;
    TlvRange parentId = data.parentId;
    TLV_TLVDATA *parent;
    /*if (parentId == TlvRange(UNDEF_TLV, UNDEF_TLV)) 
    {
      parent = dynamic_cast<TLV_TLVDATA *>(tlv);  

    } else { */
      parent = dynamic_cast<TLV_TLVDATA *>(GetFirstTlv(tlv, parentId));
//std::cout << "delete id is " << tlvRange.getLowerBound() << std::endl;
//std::cout << "delete parent id is " << parentId << std::endl;
    //}
    if (NULL != parent) {
       parent->DeleteTlvData(tlvRange.getLowerBound(), tlvRange.getUpperBound());
    }



   /* TlvRange target = tlvRange;
    bool deleteParent = true;
    ConfigData data = (configMap.find(target.getLowerBound()))->second;
    TLV_BASE *parent = tlv;
    do {
      if (NULL == parent)
        return; //doesnt exist
      //Get parent, delete child (our target)
      TLV_TLVDATA *parent = GetTlv(parent, data.parentId);
      parent->DeleteData(target.getLowerBound(), target.getUpperBound());

      //if parent is optional, delete parent
      target = data.parentId;
      if (target == TlvRange(UNDEF_TLV, UNDEF_TLV))
      {
         deleteParent = false; //at top of tree now
      }
      //parent is required, dont delete
      if (parent->required()) { exit; }
      if (!((configMap[target.getLowerBound]).isOptional))
        deleteParent = false;
    } while (deleteParent); */

  }
  void
  TlvHelperBase::DeleteTlv(TLV_BASE *tlv,   const std::string &tlvName) const
  {
    //TlvRange tlvRange = nameToIdMap[tlvName];
    std::map < std::string, TlvRange >::const_iterator it =  nameToIdMap.find(tlvName);
    TlvRange tlvRange = it->second;
    DeleteTlv(tlv, tlvRange);
  }
   TLV_BASE *
   TlvHelperBase::GetFirstTlv(TLV_BASE *tlv, TlvRange &tlvRange) const
   {
    if (NULL == tlv)
      return NULL;
    if (tlvRange == UNDEF_TLV)
      return NULL;
    //if we want a root tlv, return tlv
       std::map<tlvTypeId, ConfigData>::const_iterator it;
       it = configMap.find(tlvRange.getLowerBound());
       ConfigData data = it->second;
       //tlvTypeId parentType = data.parentId.getLowerBound();
       TlvRange parentRange = data.parentId;

       
         TlvTypeRange tlvContainerRange(tlvRange.getLowerBound(), tlvRange.getUpperBound());
          TLV_TLVDATA *selfTlv = dynamic_cast<TLV_TLVDATA *>(tlv);
    if (parentRange == UNDEF_TLV)  //we are root
    {
         TLV_TYPE_t type = tlv->GetType(); 
         if (tlvContainerRange.isInRange(type)) //we are asking for ourself
           return selfTlv;
         //we are asking for a tlv contained in root
         return selfTlv->GetFirstTlvData(tlvContainerRange);
    }
     
    //JLM FIXME TODO should have a setParentTlv by range.
     TLV_TLVDATA *parentTlv = SetParentTlvs(selfTlv, parentRange.getLowerBound(), false);
     if (parentTlv) {
         return parentTlv->GetFirstTlvData(tlvContainerRange);
     } else {
       return NULL;
     }

    /*
    ConfigData data = (configMap.find(tlvId.getLowerBound()))->second;
       tlvTypeId parentType = data.parentId.getLowerBound();
       TLV_TLVDATA *parent = dynamic_cast<TLV_TLVDATA *>(GetTlv(tlv, parentType));
     if (NULL == parent)
       return NULL;
     return parent->GetFirstTlvData(tlvId.getLowerBound(), tlvId.getUpperBound());
     */
   }
   
   TLV_BASE *
  TlvHelperBase::GetTlv( TLV_BASE *tlv, const std::string &tlvName) const
  {
      //TlvRange tlvRange = nameToIdMap[tlvName];
    std::map < std::string, TlvRange >::const_iterator it =  nameToIdMap.find(tlvName);
    TlvRange tlvRange = it->second;
      return GetFirstTlv(tlv, tlvRange);
  }


   TLV_BASE *
  TlvHelperBase::GetTlv(TLV_BASE *tlv, tlvTypeId tlvId) const
  {
    if (NULL == tlv)
      return NULL;
    if (tlvId == UNDEF_TLV)
      return NULL;

    TlvRange tlvRange(tlvId, tlvId);
    return GetFirstTlv(tlv, tlvRange);
  }


  TLV_LENGTH_t
  TlvHelperBase::GetTlvSize(TLV_BASE *tlv, const std::string &tlvName) const {
      TLV_BASE *rettlv = GetTlv(tlv, tlvName);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetLength();
  }

   TLV_LENGTH_t
   TlvHelperBase::GetTlvSize(TLV_BASE *tlv, tlvTypeId tlvId) const {
       TLV_BASE *rettlv = GetTlv(tlv, tlvId);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetLength();
  
   }
   TLV_LENGTH_t
   TlvHelperBase::GetFirstTlvSize(TLV_BASE *tlv, TlvRange &tlvRange) const {
       TLV_BASE *rettlv = GetFirstTlv(tlv, tlvRange);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetLength();
  
   }

  const TLV_RAW_DATA_t *
  TlvHelperBase::GetTlvData(TLV_BASE *tlv, const std::string &tlvName) const {
      TLV_BASE *rettlv = GetTlv(tlv, tlvName);
      if (NULL == rettlv) { return NULL; }
        return rettlv->GetData();
  
  }
  const TLV_RAW_DATA_t *
   TlvHelperBase::GetTlvData(TLV_BASE *tlv, tlvTypeId tlvId) const {
        TLV_BASE *rettlv = GetTlv(tlv, tlvId);
      if (NULL == rettlv) { return NULL; }
        return rettlv->GetData();
  
   }
   const  TLV_RAW_DATA_t *
TlvHelperBase::GetFirstTlvData(TLV_BASE *tlv, TlvRange &tlvRange) const {
        TLV_BASE *rettlv = GetFirstTlv(tlv, tlvRange);
      if (NULL == rettlv) { return NULL; }
        return rettlv->GetData();
}   

          TLV_TYPE_t
  TlvHelperBase::GetTlvType(TLV_BASE *tlv, const std::string &tlvName) const {
      TLV_BASE *rettlv = GetTlv(tlv, tlvName);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetType();
  
  }
   TLV_TYPE_t
   TlvHelperBase::GetTlvType(TLV_BASE *tlv, tlvTypeId tlvId) const {
       TLV_BASE *rettlv = GetTlv(tlv, tlvId);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetType();
   
   }
   TLV_TYPE_t
   TlvHelperBase::GetFirstTlvType(TLV_BASE *tlv, TlvRange &tlvRange) const {
        TLV_BASE *rettlv = GetFirstTlv(tlv, tlvRange);
      if (NULL == rettlv) { return 0; }
      return rettlv->GetType();
  
   }


   //static generators for Interest, Data, and InterestResponse
 const TlvHelperBase  * 
 GetDataTlvHelper() {
     static TlvHelperBase *dataHelper; 
     static int init = 1;

     if (init) {
          init = 0;
          dataHelper = new TlvHelperBase;
          dataHelper->Configure("DataTlvRules.xml");
     }
     return dataHelper;
 }



 //factories to only use the configuration file once
  const TlvHelperBase  * 
  GetInterestTlvHelper() {
     static TlvHelperBase *dataHelper;
     static int init = 1;

     if (init) {
          init = 0;
          dataHelper = new TlvHelperBase;
          dataHelper->Configure("InterestTlvRules.xml");
     }
     return dataHelper;

  }

  const TlvHelperBase  * 
  GetInterestResponseTlvHelper() {
     static TlvHelperBase *dataHelper;
     static int init = 1;

     if (init) {
          init = 0;
          dataHelper = new TlvHelperBase;
          dataHelper->Configure("InterestResponseTlvRules.xml");
     }
     return dataHelper;

  }
   
}
}
}


/*int main()
 {
   wire::icnx::tlvUtils::TlvHelperBase help;
   std::string fileName = "InterestTlvRules.xml";
   help.Configure(fileName);

    return 0;
 }
 */

