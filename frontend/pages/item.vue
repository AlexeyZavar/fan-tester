<template>
  <div class="flex justify-center">
    <div class="w-1/2 card">
      <p>{{ item.name }}</p>
      <hr>
      <div class="card">
        <p>⚡ Тяга от тока</p>
        <hr>
        <chart-thrust-amperage :amperes="item.thrust_amperes" :thrust="[12,20,22,23,24,25,67]" :height="200" />
      </div>
      <div class="card">
        <p>🔌 Тяга от мощности</p>
        <hr>
        <chart-thrust-power :power="item.thrust_power" :thrust="[12,20,22,23,24,25,67]" :height="200" />
      </div>
      <div class="card col-span-2">
        <p>🥇 Эффективность</p>
        <hr>
        <chart-efficiency :efficiency="item.efficiency" :height="200" />
      </div>
      <div class="card col-span-2">
        <p>📉 Результаты</p>
        <hr>
        <p>🔥 Время разгона: {{ item.warmup_time }} сек.</p>
        <div>
          <p>⚡ Макс. ток: {{ item.max_amperes }}</p>
          <p>✨ Макс. напряжение: {{ item.max_voltage }}</p>
          <p>✨ Мин. напряжение: {{ item.min_voltage }} </p>
          <p>🔌 Макс. мощность: {{ item.max_power }}</p>
          <p>✈️ Макс. тяга: {{ item.max_thrust }}</p>
        </div>
        <table class="mt-4 w-full table-auto">
          <thead>
            <tr class="bg-gray-50">
              <th>_</th>
              <th>T</th>
              <th>P</th>
              <th>I</th>
              <th>V</th>
              <th>t°</th>
            </tr>
          </thead>
          <tbody class="">
            <tr>
              <th>5%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>10%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>15%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>20%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>25%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
            <tr>
              <th>30%</th>
              <th>11</th>
              <th>22</th>
              <th>33</th>
              <th>44</th>
              <th>55</th>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import Vue from 'vue'
import { DatabaseItem } from '~/pages/database.vue'

export default Vue.extend({
  name: 'ItemPage',
  async asyncData ({ $axios, route }) {
    const item = await $axios.$get(`/database/${route.query.name}`)
    return { item }
  },
  data () {
    // @ts-ignore
    const item: DatabaseItem = undefined

    return {
      item
    }
  }
})
</script>
