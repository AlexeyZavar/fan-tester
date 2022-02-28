<script lang="ts">
// eslint-disable-next-line import/named
import { Line, mixins } from 'vue-chartjs'
import Vue from 'vue'

export default Vue.extend({
  name: 'ChartThrustAmperage',
  extends: Line,
  mixins: [mixins.reactiveProp],
  props: ['amperes', 'thrust'],
  mounted () {
    // @ts-ignore
    this.renderChart({ labels: this.amperes, datasets: [{ label: 'Тяга', fill: false, lineTension: 0.1, borderColor: '#c4b5fd', data: this.thrust }] })
  }
})
</script>

<style scoped>

</style>
