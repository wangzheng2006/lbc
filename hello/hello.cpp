#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/dispatcher.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]]  hello : public eosio::contract {

public:
    using contract::contract;

    [[eosio::action]]
    void hi(name user) {
        print("game start");
        require_auth(_self);
        auto values = user.value;
        print("hello userS  value:", values);
        print(",: Hello ,: ", user);
        action(permission_level{_self, name("active")},
               name("eosio.token"), name("transfer"),
               std::make_tuple(_self, user, asset(1, symbol("EOS", 4)),
                               std::string("game send  eos"))).send();
    }

    [[eosio::action]]
    void delay(string memo) {
        eosio::transaction t{};
        t.actions.emplace_back(eosio::permission_level(_self, name("active")),
                               name("test3"), name("hi"),
                               std::make_tuple(name("test1")));
        t.delay_sec = 1;
        t.send(1, _self, false);
        print("delay end");
    }

    [[eosio::action]]
    void transfer(name from, name to, asset quantity, string memo) {
        //taskid:1
        //memo里面附带附加信息
        print("transfer from ", from);
        string::size_type idx;
        idx =memo.find("taskid:");
        if(idx!=string::npos){
            string id=memo.substr(memo.find("taskid:")+7,memo.length());
            print(" id:",id.c_str());

            task_index tasks(_self,_self.value);

            auto taskitem=tasks.find(atoi(id.c_str()));

            if(taskitem!=tasks.end()){
//                asset pay=taskitem->reward;
//                eosio_assert(quantity.amount<pay.amount ,"transfer error");

                taskuser_index  tasku(_self,from.value);
                auto taskUserItem=tasku.find(taskitem->id);
                if(taskUserItem==tasku.end()){
                    tasku.emplace(_self,[&](auto &t){
                       t.taskid=taskitem->id;
                       t.is_end=0;
                       t.start_time=current_time();
                    });
                }


            }

        }

    }

    [[eosio::action]]
    void add(name from, uint64_t phone) {
        work_index works(_self, _self.value);
        works.emplace(_self, [&](auto &work) {
            work.worker = from;
            work.phone = phone;
            work.score = asset(1, symbol("EOS", 4));
        });
    }

    [[eosio::action]]
    void update(name from, uint64_t phone) {
        work_index works(_self, _self.value);
        auto item = works.find(from.value);
        works.modify(item, _self, [&](auto &work) {
            work.phone = phone;
        });
    }

    [[eosio::action]]
    void del(name from) {

        work_index works(_self, _self.value);
        auto item = works.find(from.value);
        works.erase(item);

    }

    [[eosio::action]]
    void query(uint64_t phone) {
        work_index works(_self, _self.value);
        auto workitem = works.get_index<"phone"_n>();
        auto item = workitem.find(phone);
        print("query name is:", item->worker);

    }

    [[eosio::action]]
    void adds(name from) {
        test_index t(_self, _self.value);
        t.emplace(_self, [&](auto &test) {
            test.id = t.available_primary_key();
        });
        print("add end");
    }


    [[eosio::action]]
    void addtask(string co, uint32_t levle, uint64_t end, asset token) {
        require_auth(_self);
        task_index tasks(_self, _self.value);
        tasks.emplace(_self, [&](auto &t) {
            t.id = tasks.available_primary_key();
            t.level = levle;
            t.contenct = co;
            t.end_time = end;
            t.reward = token;
        });

    }

    struct [[eosio::table]]  work {
        name worker;//姓名
        asset score;//token数量
        uint64_t phone;//电话
        uint64_t reward;
        uint64_t exchange;

        //主键
        uint64_t primary_key() const { return worker.value; }

        //二级索引
        uint64_t get_phone() const { return phone; }

        //序列化数据。
        EOSLIB_SERIALIZE( work, (worker)(score)(phone)(reward)(exchange)
        )
    };

    //参数1 表名称 最长 12 个字符，只能由小 写字母、数字 1-5、“.”组成。
    //参数2 表对象 表中 行的定义
    //参数3 可变参数index  最多支持16个 必须返回结构中定义的常量类型，返回二级索引或者引用
    typedef eosio::multi_index<name("work"), work,
            indexed_by < "phone"_n, const_mem_fun < work, uint64_t, &work::get_phone>>>
    work_index;


    struct [[eosio::table]] test {
        uint64_t id;

        uint64_t primary_key() const { return id; }

        EOSLIB_SERIALIZE( test, (id)
        )
    };

    typedef eosio::multi_index<name("test"), test> test_index;

    struct [[eosio::table]] task {
        uint64_t id;
        string contenct;
        uint32_t level;
        uint64_t end_time;
        asset reward = asset(0, symbol("EOS", 4));
        uint64_t primary_key() const { return id; }

        EOSLIB_SERIALIZE(task, (id)(contenct)(level)(end_time)(reward)
        )
    };
    typedef eosio::multi_index<name("task"), task> task_index;

    struct [[eosio::table]] taskuser{
        uint64_t taskid;
        uint32_t is_end;//0 :未结束 1：已结束
        uint64_t start_time;//认领日期
        uint64_t end_time;//提交日期
        asset reward = asset(0, symbol("EOS", 4));
        uint64_t primary_key() const { return taskid; }

        EOSLIB_SERIALIZE(taskuser, (taskid)(is_end)(start_time)(end_time)(reward)
        )
    };
    typedef eosio::multi_index<name("taskuser"), taskuser> taskuser_index;

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


EOSIO_DISPATCH_CUSTOM(hello, (hi)(addtask)(query)(delay)(transfer)(add)(update)(del)(adds))

