import { createApp } from 'vue'
import App from './App'
import axios from 'axios'
const app = createApp(App)

app.use(axios)
app.mount('#app')
