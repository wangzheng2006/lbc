<template>
  <div class="game-center">
    <div class="item-title">任务中心</div>
    <div class="theme-list">
      <div class="theme-item" v-for="item in themeList" :key="item.title"  @click="receive(item.id)" > 
        <i class="iconfont e-Shape" v-if="item.isHot" />
        <i class="iconfont e-xingzhuang" v-else />
        <div class="title">{{item.contenct}}</div>
        <div class="num">{{item.reward}} </div>
      </div>
    </div>

     <infos  v-bind:contenct="contenct" v-bind:id="id" v-bind:reward="reward" v-if="showInfo" @close="() => showInfo = false" />

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
import infos from "./gameInfo.vue";
import Scatterutil from "../../utils/scatterutil";


export default {
  
  data() {
    return {
      // 热门任务数据
      themeList: [
        {
          contenct: "测试任务?",
          reward: "100 EOS",
          isHot: true,
          route:""
        },
        {
          contenct: "开发任务?",
          reward: "200 EOS",
          isHot: false,
          route:""
        }
      ],
      log:"",
      id:"",
      contenct:"",
      reward:"",
      showInfo:false,
      showLoading:false
    };
  },
  mounted() {
    this.init();
  },
  components: {
    loading,
    infos
  },
  methods: {
    init() {
      var obj=this;

      rpc.get_table_rows({
        code:"test3",
        scope:"test3",
        limit:20,
        table:"task"
      }).then(result=>{
        obj.themeList=result.rows;
      })

      Scatterutil.init().then(function(data) {
        alert("初始化成功！");
      });

      // rpc.get_block(1).then(result => 
      // syntax(result,obj)
      // )

      // rpc.get_account("test3").then(result => 
      // syntax(result,obj))

      // rpc.get_currency_balance("eosio.token","test3","EOS").then(result => 
      // syntax(result,obj))

      // rpc.get_currency_stats("eosio.token","EOS").then(result => 
      // syntax(result,obj))
    },
    receive(id){
      this.showInfo=true;
      this.id=id;
      this.contenct=this.themeList[id].contenct;
      this.reward=this.themeList[id].reward;
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
.game-center {
  width: 750px;
  margin: 0 auto;
  padding: 0 20px 140px;
  .item-title {
    font-size: 32px;
    margin-top: 40px;
  }
  /* 热门话题 */
  .theme-list {
    margin-top: 10px;
    padding: 25px 0 25px 30px;
    background: #fff;
    box-shadow: 0px 4px 14px 0px rgba(0, 0, 0, 0.05);
    border-radius: 10px;
    .theme-item {
      display: flex;
      align-items: center;
      height: 60px;
      font-size: 30px;
      color: #233243;
      .iconfont {
        font-size: 26px;
        color: #4a87fb;
      }
      .e-Shape {
        color: #f02d63;
      }
      .title {
        margin-left: 14px;
        max-width: 440px;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
      }
      .num {
        margin-left: 14px;
        font-size: 26px;
        color: #8993a4;
      }
    }
  }
  /* 更多好玩 */
  .more-list {
    margin-top: 10px;
    display: flex;
    flex-wrap: wrap;
    justify-content: space-between;
    .game-item {
      display: block;
      text-decoration: none;
      width: 345px;
      height: 200px;
      border-radius: 10px;
      margin-bottom: 20px;
      &:nth-child(1) {
        background: url("../../assets/images/game1.jpg") no-repeat center;
        background-size: contain;
      }
      &:nth-child(2) {
        background: url("../../assets/images/game2.jpg") no-repeat center;
        background-size: contain;
      }
      &:nth-child(3) {
        background: url("../../assets/images/game3.jpg") no-repeat center;
        background-size: contain;
      }

      .title {
        font-size: 32px;
        color: #fff;
        margin-top: 40px;
        padding-left: 30px;
      }
      em {
        padding: 0 8px;
      }
      .info {
        margin-top: 6px;
        color: rgba(255, 255, 255, 0.6);
        font-size: 26px;
        padding-left: 30px;
      }
    }
  }
}
</style>
