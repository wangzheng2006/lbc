<template>
  <div class="" style="width:100%">
    <pre id="result">
<div v-html="log" class="item-title" 
            style="width:100%; font-size: 16px;
            white-space:normal;
            word-break:break-all;
            word-wrap:break-word; ">
            {{log}}</div>
      </pre>

  </div>
</template>

<script>
import loading from "@/components/Home/loading.vue";
// import * as Eos from 'eosjs'
import { Api, JsonRpc, RpcError } from 'eosjs';
import { JsSignatureProvider } from 'eosjs/dist/eosjs-jssig';           // development only
const defaultPrivateKey = "5JKFSKvJVjgRn4n3QRPoy4hSg76qZPXVnKuNcr3yhvXhpdMd5q2"; // bob
const signatureProvider = new JsSignatureProvider([defaultPrivateKey]);
const rpc = new JsonRpc('http://150.109.41.13:8888');
const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() });

// let eos = null;
// const apis = Eos({
//     httpEndpoint: "http://150.109.41.13:8888",
//     chainId: "cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f"
// });
// var config = {
//   keyProvider: ['5JKFSKvJVjgRn4n3QRPoy4hSg76qZPXVnKuNcr3yhvXhpdMd5q2',
//   '5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3'], // WIF string or array of keys..
//   httpEndpoint: 'http://150.109.41.13:8888',
//   	port: 8888,
//   expireInSeconds: 60,
//   broadcast: true,
//   debug: true,
//   sign: true,
//   authorization: null // 该参数用于在多签名情况下，识别签名帐号与权限,格式如：account@permission
// }
 
// eos = Eos.Localnet(config);

export default {
  
  data() {
    return {
      log:"",
      showLoading:false
    };
  },
  mounted() {
    this.init();
  },
  components: {
    loading
  },
  methods: {
    init() {
     var options = {broadcast: true} //不广播此笔交易
    api.transact({
          actions: [{
            account: 'eosio.token',
            name: 'transfer',
            authorization: [{
              actor: 'test3',
              permission: 'active',
            }],
            data: {
              from: 'test3',
              to: 'test1',
              quantity:'0.0001 EOS',
              memo: '123123',
            },
          }]
        }, {
          blocksBehind: 3,
          expireSeconds: 30,
        }).then(result => {
					console.log(` Transaction ID: ${result.transaction_id}`);

				}).catch(error => {
					console.log(error);
				});
     
      // api.transfer({from: 'test3', to: 'test1', quantity: '1.0000 EOS', memo: ''}, 
      // options,function(data){
      //     alert("test");
      //     alert("ttt:"+data);
      // }) //从test3发送1个EOS到test1账户
      // alert("sdsd");

      var obj=this;
      // rpc.get_info({}).then(result => 
      // syntax(result,obj)
      // )

      rpc.get_block(1).then(result => 
      syntax(result,obj)
      )

      rpc.get_account("test3").then(result => 
      syntax(result,obj))

      rpc.get_currency_balance("eosio.token","test3","EOS").then(result => 
      syntax(result,obj))

      rpc.get_currency_stats("eosio.token","EOS").then(result => 
      syntax(result,obj))
    },
    showgame(id) {
      this.$router.push("/themeDetail/"+id);
    }
  },
  filters: {
   
  }
};

function syntax(json,obj) {
  
 console.log(json);
    if (typeof json != 'string') {
        json = JSON.stringify(json, undefined, 2);
    }
    json = json.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    var htmls="";
    return json.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, function(match) {
        var cls = 'number';
        if (/^"/.test(match)) {
            if (/:$/.test(match)) {
                cls = 'key';
            } else {
                cls = 'string';
            }
        } else if (/true|false/.test(match)) {
            cls = 'boolean';
        } else if (/null/.test(match)) {
            cls = 'null';
        }
        if(cls!="key"){
         htmls+= '<span class="' + cls + '">' + match + '</span></br>';
        }else{
        htmls+= '<span class="' + cls + '">' + match + '</span>';
        }
    obj.log=htmls;
    });
}
</script>


<style lang="scss">
 pre {outline: 1px solid #ccc; padding: 5px; margin: 5px; }
    .string { color: green; }
    .number { color: darkorange; }
    .boolean { color: blue; }
    .null { color: magenta; }
    .key { color: red;width: 100%; }
  .item-title {
    font-size: 32px;
  }
</style>
