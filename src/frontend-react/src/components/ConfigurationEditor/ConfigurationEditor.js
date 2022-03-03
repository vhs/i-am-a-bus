import React, { Component } from 'react'
import PropTypes from 'prop-types'

import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'
import Button from 'react-bootstrap/Button'
// import Input from 'react-bootstrap/Input'
import Form from 'react-bootstrap/Form'

import './ConfigurationEditor.css'

class ConfigurationEditor extends Component {
  constructor (props) {
    super(props)

    this.state = { ...{ newText: '', config: { default: '', texts: { options: [] } } }, ...props }
  }

  componentDidMount () {
    this.setState({ newText: '' })
  }

  static getDerivedStateFromProps (props, state) {
    let updateObj = {}

    if (JSON.stringify(state.config) !== JSON.stringify(props.config)) {
      const { config } = props
      updateObj = { ...updateObj, ...{ config } }
    }

    return updateObj
  }

  addSignTextOption () {
    if (this.state.newText === undefined || this.state.newText === '') { return alert('Error: No or invalid text') }

    this.setState((currentState) => {
      const newState = Object.assign({}, currentState)

      if (this.state.newText !== '' && newState.config.texts.options.indexOf(this.state.newText) === -1) { newState.config.texts.options.push(this.state.newText) }

      newState.newText = ''

      return newState
    })
  }

  removeSignTextOption (idx) {
    if (this.state.config.texts.options[idx] === undefined) { return alert('Error: Invalid text selected') }

    this.setState((currentState) => {
      const newState = Object.assign({}, currentState)
      if (newState.config.texts.options[idx] !== undefined) newState.config.texts.options.splice(idx, 1)

      return newState
    })
  }

  updateSignTextOption (idx, text) {
    if (text === '') { return alert('Error: Empty text') }

    this.setState((currentState) => {
      const newState = Object.assign({}, currentState)
      newState.config.texts.options[idx] = text

      return newState
    })
  }

  updateDefaultText (text) {
    if (text === '') { return alert('Error: Empty text') }

    this.setState((currentState) => {
      const newState = Object.assign({}, currentState)
      newState.config.texts.default = text

      return newState
    })
  }

  updateConfig () {
    try {
      fetch('/api/updateconfig', {
        method: 'POST',
        mode: 'no-cors',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: 'config=' + encodeURIComponent(JSON.stringify(this.state.config))
      })
    } catch (err) {
      return alert('Error: ' + err)
    }
  }

  render () {
    const DefaultTextOptions = this.state.config.texts.options.map((textOption, idx) => {
      const selected = this.state.config.texts.default === textOption
      return (<option key={idx} value={textOption} selected={selected}>{textOption}</option>)
    })

    const Options = this.state.config.texts.options.map((option, idx) => {
      return (
        <Row className="spacious" key={idx}>
          <Col>
            <Form.Control type='text' id={'LEDTextOption' + idx} value={option} onChange={(e) => this.updateSignTextOption(idx, e.target.value)} />
          </Col>
          <Col xs={12} md={4}>
            <Button className="btn-warning" onClick={() => this.removeSignTextOption(idx)}>Remove</Button>
          </Col>
        </Row>
      )
    })

    return (
      <>
        <Row>
          <Col>
            <h2>Default Text</h2>
          </Col>
        </Row>
        <Row>
          <Col>
            <Form.Control as="select" onChange={(e) => this.updateDefaultText(e.target.value)}>
              <option value={this.state.config.texts.default}>{this.state.config.texts.default}</option>
              {DefaultTextOptions}
            </Form.Control>
          </Col>
        </Row>
        <Row>
          <Col>
            <h2>Text Options</h2>
          </Col>
        </Row>
        <Row>
          <Col>
            {Options}
          </Col>
        </Row>
        <Row>
          <Col xs={12} md={8}>
            <Form.Control type='text' id='newLEDTextOption' onChange={(e) => { this.setState({ newText: e.target.value }) }} value={this.state.newText} />
          </Col>
          <Col xs={12} md={4}>
            <Button onClick={() => this.addSignTextOption()}>Add Text</Button>
          </Col>
        </Row>
        <Row className="spacious">
          <Col>
            <Button onClick={() => this.updateConfig()}>Update config</Button>
          </Col>
        </Row>
      </>
    )
  }
}

ConfigurationEditor.propTypes = {
  config: PropTypes.array.isRequired,
  updateConfig: PropTypes.func.isRequired
}

ConfigurationEditor.defaultProps = {}

export default ConfigurationEditor
