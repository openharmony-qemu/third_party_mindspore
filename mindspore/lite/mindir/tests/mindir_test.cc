//
// Created by tony on 9/19/22.
//
#include "mindir.h"
#include "mindir_memory_manager.h"
#include <iostream>
using namespace mindspore::lite;
int main() {
  int loop = 0;
  auto tensor = MindIR_Tensor_Create();
  auto str = MindIR_Tensor_GetName(tensor);
  DataType dataType = DATA_TYPE_UINT16;
  MindIR_Tensor_SetDataType(&tensor, dataType);
  std::cout << "set data type" << std::endl;
  DataType d = MindIR_Tensor_GetDataType(tensor);
  std::cout << "loop = " << loop << ",data type = " << (int)d << std::endl;
  (void)MindIR_Activation_CreatePrimitive(ACTIVATION_TYPE_RELU6, .5, 0, 1, true);
  std::cout << "MindIR_Activation_CreatePrimitive 1" << std::endl;
  (void)MindIR_Activation_CreatePrimitive(ACTIVATION_TYPE_RELU6, .5, 0, 1, true);
  std::cout << "MindIR_Activation_CreatePrimitive 2" << std::endl;
  MindIRMemoryManager::GetInstance()->ClearAllMemory();
  std::cout << "MindIRMemoryManager::GetInstance()->ClearAllMemory()" << std::endl;
  loop++;
}