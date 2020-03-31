/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::cout << "name:" << request->name() << std::endl;
    std::cout << "age:" << request->age() << std::endl;
    std::cout << "addr:" << request->addr() << std::endl;
    std::cout << "corpus:" << request->corpus() << std::endl;

    std::cout << "math:" << request->score().math() << std::endl;
    int size = request->desc_size();
    std::cout << "size:" << size << std::endl;
    for(int i=0; i<size; i++) {
      helloworld::Desc item = request->desc(i);
      std::cout << "i:" << i << ",test:" << item.test() << std::endl;
    }

    std::cout << "url:" << request->result().url() << std::endl; 
    std::cout << "title:" << request->result().title() << std::endl; 

    std::cout << "bool:" << request->booly() << std::endl;

    int map_size = request->map_array_size();
    std::cout << "map_size:" << map_size << std::endl;
    //转换成标准map
    std::map<std::string, std::string> standard_map(request->map_array().begin(),
                                    request->map_array().end());
    std::map<std::string, std::string>::iterator iter;
    for(iter = standard_map.begin(); iter != standard_map.end();)
    {
      std::string key = iter->first;
      std::cout << "key:" << key << std::endl;
      std::string value = iter->second;
      std::cout << "value:" << value << std::endl;
      ++iter;
    } 

    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    reply->set_code(200);
    return Status::OK;
  }
};

void RunServer() {
  //std::string server_address("0.0.0.0:50051");
  std::string server_address("127.0.0.1:51");
  
  // 服务构建器，用于构建同步或者异步服务
  ServerBuilder builder;
  // 添加监听的地址和端口，后一个参数用于设置认证方式，这里选择不认证
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // 创建服务对象
  GreeterServiceImpl service;
  // 注册服务
  builder.RegisterService(&service);
  // 构建服务器
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // 进入服务处理循环（必须在某处调用server->Shutdown()才会返回）
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
