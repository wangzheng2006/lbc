#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
//引入必要依赖包
#include <eosiolib/dispatcher.hpp>

//命名空间
using namespace eosio;
using namespace std;
//合约必须继承至contract
class hello : public contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void hi( name user ) {
		print("game start");
		//校验_self 权限
		require_auth(_self);
		auto values=user.value;
        	print("hello user value:",values);
		print( ",: Hello ,: ", user);

		//内联调用 语法说明
		//action(
		//permission_level, //权限
		//code, 合约
		//action, 合约方法
		//data); 调用数据
      		action(permission_level{_self, name("active")},
              	name("eosio.token"), name("transfer"),
                std::make_tuple(_self, user, asset(1, symbol("EOS", 4)),
                                std::string("game send  eos")) ).send();
	}
	
	[[eosio::action]]
	void delay(string memo){

		 eosio::transaction t{};//声明 transaction
		 	//封装数据
       		 t.actions.emplace_back(eosio::permission_level(_self, name("active")),
        	                       name("test3"), name("hi"),
                               std::make_tuple(name("test1")));
            //设置延时时间
        	t.delay_sec = 1;
        	//发送交易数据
        	t.send(1, _self, false);
		print("delay end");
	}
	[[eosio::action]]
	void transfer(name from, name to, asset quantity, string memo) {
		print("transfer from ",from);
		// eosio_assert 如果判断条件不成立，则终止执行并打印错误信息（真实是直接不通过没有打印）
		eosio_assert(0," transfer error");


		}


};
 /**
 * 合约调用入口
 * push action 都是传到这个函数里面进行处理
 * apply动作处理程序是一个侦听所有传入操作并执行所需行为的函数
 * receiver, code, action。 receiver是当前正在处理该操作的帐户。code 是授权合约的帐户。 action是当前合约的方法。
 * 每个智能合约都必须提供应用操作处理程序
 * EOSIO_DISPATCH_CUSTOM 宏 封装了 apply
 */

#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) ) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \


EOSIO_DISPATCH_CUSTOM( hello, (hi)(delay)(transfer))


