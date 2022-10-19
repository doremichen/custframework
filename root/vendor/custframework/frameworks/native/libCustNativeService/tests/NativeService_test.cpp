//*******************************************
// FILE NAME:   NativeService_test.cpp
// VERSION:     $Revision: $
// DESCRIPTION: Use google test to verify Native Service
// 
//
//*****************************************************************************
// UPDATE LOG:
// $Log: $
//*****************************************************************************

#include <stdio.h>

#include <NativeService.h>

#include "gtest/gtest.h"

namespace android {
    
class NativeServiceTest : public testing::Test {
protected:
      virtual void SetUp() 
      {
//        printf("[%s] enter\n", __FUNCTION__);
        pService = new NativeService;
         
      }
      
      virtual void TearDown() 
      { 
//        printf("[%s] enter\n", __FUNCTION__);
        if(pService)
            delete pService;
      }


      NativeService *pService;      
      
};
    
int On(NativeService *service)
{
   int ret = 0; 
    
//   printf("[%s] enter\n", __FUNCTION__);

   ret = service -> module_on();
   
   return ret; 
}

int Off(NativeService *service)
{
    int ret = 0; 
    
//    printf("[%s] enter\n", __FUNCTION__);
    ret = service -> module_off();
       
    return ret;
}  


TEST_F(NativeServiceTest, testOn) 
{    
    int expected = 1;
    
    EXPECT_EQ(expected, On(pService));
}    

TEST_F(NativeServiceTest, testOff) 
{    
    int expected = 1;
    
    EXPECT_EQ(expected, Off(pService));
}   

}    


int main(int argc, char** argv)
{
    
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
