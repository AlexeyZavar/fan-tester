<template>
  <div class="flex justify-center">
    <div class="w-7/12 flex flex-col gap-8">
      <div class="card">
        <p>⚙️ Настройки и запуск</p>
        <hr>
        <input v-model="name" :disabled="!!state.length" class="inp" placeholder="Название" type="text">
        <div class="flex flex-row items-center space-x-2">
          <input v-model="soft_start" :disabled="!!state.length" type="checkbox">
          <p>💡 Мягкий старт</p>
        </div>
        <hr>
        <button :disabled="!!state.length" class="btn" @click="startBenchmark">
          Запустить бенчмарк
        </button>
        <button :disabled="!!state.length" class="btn" @click="calibrate">
          ⚡ Откалибровать ⚡
        </button>
      </div>
      <div v-if="state.length" class="grid grid-cols-2 gap-8">
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
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import Vue from 'vue'

interface State {
  pwm: number
  current_rpm: number
  current_amperes: number
  current_voltage: number
  tensometer1: number
  tensometer2: number
  tensometer3: number
  running: boolean
}

export default Vue.extend({
  name: 'BenchmarkPage',
  data () {
    const state: State[] = []

    return {
      name: '',
      soft_start: true,
      state
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
    }
  },
  methods: {
    async startBenchmark () {
      if (!this.name) {
        return
      }

      await this.$axios.post('/benchmark', { name: this.name, soft_start: this.soft_start })

      const pooling = setInterval(async () => {
        try {
          this.state = await this.$axios.$get('/benchmark')

          console.log(this.state)

          if (!this.state[this.state.length - 1].running) {
            clearInterval(pooling)
            this.state = []
          }
        } catch {
          clearInterval(pooling)
          this.state = []
        }
      }, 1500)
    },
    async calibrate () {
      await this.$axios.post('/calibrate')
    }
  }
})
</script>

<style scoped>

</style>
