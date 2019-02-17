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
class [[eosio:contract]] hello : public contract {
  public:
      using contract::contract;

	  // 声明action
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

	[[eosio::action]]
		void add(name from,uint64_t phone){

      	//构造函数 multi_index(uint64_t code, uint64_t scope)
		//参数1 code拥有表的账户，可以对数据库有读写权限。
		//参数2  scope 范围标识符。
		work_index  works(_self,_self.value);

		works.emplace(_self, [&](auto& work) {
			work.account_name = from;
			work.score = asset(1,symbol("EOS", 4));
			work.phone = phone;
		});
      }

	[[eosio::action]]
	void update(name from ,uint64_t phone){
		work_index works(_self,_self.value);
		auto item=works.find(from.value);
		if(item!=works.end()){
			works.modify(item,_self,[&](auto& work){
				work.phone=phone;
			});
		}
	}

	[[eosio::action]]
	void query(uint64_t phone){
		work_index works(_self,_self.value);
		auto workeritem= works.get_index<"phone"_n>();
		auto item=workeritem.find(phone);
		print("query name:",item->worker);

	}


	[[eosio::action]]
	void adds(name from){
		test_index t(_self,_self.value);
		t.emplace(_self,[&](auto& test){
			test.id=t.available_primary_key();
		});
		print("add end");
	}

	struct [[eosio::table]] work{
		uint64_t id;
		name worker;
		asset token;
		uint64_t score;
		uint64_t creat_time;
		//主键
		uint64_t primary_key() const {  return id; }

		//序列化数据。
		EOSLIB_SERIALIZE( work, (id)(worker)(token)(score)(creat_time))
	};

	struct [[eosio::table]] work{
		name worker;//姓名
		asset score;//token数量
		uint64_t phone;//电话
		//主键
		uint64_t primary_key() const {  return worker.value; }
		//二级索引
		uint64_t get_phone() const { return phone; }
		//序列化数据。
		EOSLIB_SERIALIZE( work, (worker)(score)(phone))
	};

	//参数1 表名称 最长 12 个字符，只能由小 写字母、数字 1-5、“.”组成。
	//参数2 表对象 表中 行的定义
	//参数3 可变参数index  最多支持16个 必须返回结构中定义的常量类型，返回二级索引或者引用
	typedef eosio::multi_index<"work"_n, work ,
			indexed_by<"phone"_n, const_mem_fun<work, uint64_t, &work:: get_phone>>> work_index;

	struct [[eosio::table]] test{
		uint64_t id;
		uint64_t primary_key() const { return id;}
		EOSLIB_SERIALIZE( test, (id))
	};
	typedef eosio::multi_index<name("test"),test > test_index;
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


EOSIO_DISPATCH_CUSTOM( hello,(hi)(delay)(transfer)(add)(update)(del)(adds))


