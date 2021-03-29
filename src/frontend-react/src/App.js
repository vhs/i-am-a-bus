import React, { Component } from 'react'

import Container from 'react-bootstrap/Container'
import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'
import Tabs from 'react-bootstrap/Tabs'
import Tab from 'react-bootstrap/Tab'

import Conditional from './components/Conditional/Conditional'
import LEDTextSelector from './components/LEDTextSelector/LEDTextSelector'
import ConfigurationEditor from './components/ConfigurationEditor/ConfigurationEditor'

import './App.css'

class App extends Component {
  constructor (props) {
    super(props)

    this.state = { config: { texts: { options: [] } }, error: null }

    this.updateConfig = this.updateConfig.bind(this)
  }

  componentDidMount () {
    try {
      fetch('/config.json')
        .then((result) => result.json())
        .then((config) => {
          this.setState({ config })
        })
    } catch (err) {
      console.log('Error fetching config file:')
      console.log(err)
      this.setState({ error: 'We encountered an unexpected error while fetching config file' })
    }
  }

  updateConfig (config) {
    this.setState({ config })
  }

  render () {
    return (
      <Container>
        <Conditional condition={this.state.error !== null}>
          <Row><Col>{this.state.error}</Col></Row>
        </Conditional>

        <Row>
          <Col>
            <Tabs defaultActiveKey="textselector">
              <Tab eventKey="textselector" title="Select Text">
                <LEDTextSelector options={this.state.config.texts.options} />
              </Tab>
              <Tab eventKey="textupdater" title="Update Config">
                <ConfigurationEditor config={this.state.config} updateConfig={this.updateConfig} />
              </Tab>
            </Tabs>
          </Col>
        </Row>
      </Container>
    )
  }
}

export default App
