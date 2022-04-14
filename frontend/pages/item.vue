<template>
  <div class="flex justify-center">
    <div class="w-1/2 card">
      <p>{{ item.name }}</p>
      <hr>
      <div class="card">
        <p>🔌 Тяга от мощности</p>
        <hr>
        <chart-thrust-power :chart-data="thrust_power_data" />
      </div>
      <div class="card">
        <p>⚡ Тяга от тока</p>
        <hr>
        <chart-thrust-amperage :chart-data="thrust_amperage_data" />
      </div>
      <div class="card col-span-2">
        <p>📉 Результаты</p>
        <hr>
        <p>🔥 Время разгона: {{ item.warmup_time.toFixed(4) }} сек.</p>
        <div>
          <p>⚡ Макс. ток: {{ max_amperes }} А</p>
          <p>✨ Макс. напряжение: {{ max_voltage }} В</p>
          <p>✨ Мин. напряжение: {{ min_voltage }} В</p>
          <p>🔌 Макс. мощность: {{ max_power }} Вт</p>
          <p>✈️ Макс. тяга: {{ max_thrust }} кг</p>
        </div>
        <table class="mt-4 w-full table-auto">
          <thead>
            <tr class="bg-gray-50">
              <th>_</th>
              <th>T (г)</th>
              <th>P (Вт)</th>
              <th>I (А)</th>
              <th>U (В)</th>
              <th>t°</th>
            </tr>
          </thead>
          <tbody class="">
            <tr v-for="(state_item, i) in state" :key="i">
              <th>{{ ((state_item.pwm - 1000) / 1000 * 100).toFixed(0) }} %</th>
              <th>{{ state_item.tensometer1 }}</th>
              <th>{{ (state_item.current_voltage * state_item.current_amperes).toFixed(4) }}</th>
              <th>{{ state_item.current_amperes.toFixed(3) }}</th>
              <th>{{ state_item.current_voltage.toFixed(3) }}</th>
              <th>-</th>
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
    const item = await $axios.$get(`/database/${route.query.id}`)
    return { item, state: item.data }
  },
  data () {
    // @ts-ignore
    const item: DatabaseItem = undefined

    return {
      item,
      state: []
    }
  },
  computed: {
    amperes () {
      const res = []

      for (const item of this.state) {
        res.push(item.current_amperes)
      }

      return res
    },
    power () {
      const res = []

      for (const item of this.state) {
        const power = item.current_voltage * item.current_amperes
        res.push(power)
      }

      return res
    },
    thrust () {
      const res = []

      for (const item of this.state) {
        res.push(item.tensometer1)
      }

      return res
    },
    efficiency () {
      const res = []

      const thrust = this.thrust
      const power = this.power
      for (let i = 0; i < this.state.length; ++i) {
        res.push(thrust[i] / power[i])
      }

      return res
    },
    thrust_power_data () {
      return {
        labels: this.thrust,
        datasets: [{ label: 'Мощность (Вт)', fill: false, lineTension: 0.1, borderColor: '#67e8f9', data: this.power }]
      }
    },
    thrust_amperage_data () {
      return {
        labels: this.thrust,
        datasets: [{ label: 'Ток (А)', fill: false, lineTension: 0.1, borderColor: '#67baf9', data: this.amperes }]
      }
    },
    max_amperes () {
      return Math.max(...this.amperes)
    },
    max_voltage () {
      return Math.max(...this.state.map(item => item.current_voltage))
    },
    min_voltage () {
      return Math.min(...this.state.map(item => item.current_voltage))
    },
    max_power () {
      return Math.max(...this.power)
    },
    max_thrust () {
      return Math.max(...this.thrust)
    }
  }
})
</script>
