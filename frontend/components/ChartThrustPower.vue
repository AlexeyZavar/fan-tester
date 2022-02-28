<script lang="ts">
// eslint-disable-next-line import/named
import { Line, mixins } from 'vue-chartjs'
import Vue from 'vue'

export default Vue.extend({
  name: 'ChartThrustPower',
  extends: Line,
  mixins: [mixins.reactiveProp],
  props: ['power', 'thrust'],
  mounted () {
    // @ts-ignore
    this.renderChart({ labels: this.power, datasets: [{ label: 'Тяга', fill: false, lineTension: 0.1, borderColor: '#67e8f9', data: this.thrust }] })
  }
})
</script>

<style scoped>

</style>
