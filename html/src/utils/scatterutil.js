import Vue from 'vue'
//加载scatter
import ScatterJS from 'scatterjs-core';
import ScatterEOS from 'scatterjs-plugin-eosjs2';

import { Api, JsonRpc, RpcError } from 'eosjs';

//初始化对象（使用eosj 或者scattereth，或者scattertron）
ScatterJS.plugins(new ScatterEOS());

let scatter = null;
let eosapi;

const network = {
    blockchain: 'eos',
    host: '150.109.41.13',
    port: 8888,
    protocol: 'http',
    chainId: 'cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f'
};

const rpc = new JsonRpc('http://150.109.41.13:8888');

const requiredFields = { accounts: [network] };

//export命令定义了模块的对外接口
export default class Scatterutil {

    static async init() {
        //ES6 Promise
        // return new Promise(async resolve => {
        //     ScatterJS.scatter.connect("kpigame").then(connected => {
        //         if (!connected) {
        //             resolve("false");
        //             return false;
        //         }
        //         scatter = ScatterJS.scatter;
        //         //
        //         eosapi = scatter.eos(network, Api, { rpc, beta3: true });
        //         // 必须清空对象防止泄露对象。
        //         window.scatter = null;
        //         resolve("true");
        //     })
        // });
    }

    //获取授权用户
    static async userinfo() {

        // return new Promise(async resolve => {
        //     scatter.getIdentity(requiredFields).then(() => {
        //         let account = { name: "", eos: 0 };
        //         if (scatter != null) {
        //             if (scatter.identity != null) {
        //                 //获取EOS的account
        //                 const accounts = scatter.identity.accounts.find(x => x.blockchain === 'eos');
        //                 account.name = accounts.name;
        //             }
        //         }
        //         resolve(account);

        //     }).catch(error => {
        //         resolve("");
        //         console.error(error);
        //     });
        // });
    };

    //转账
    static async transfer(num, memo) {
        // if (scatter == null) {
        //     alert("请安装scatter插件！");
        // }
       
    };


}
