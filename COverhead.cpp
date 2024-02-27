#include "COverhead.h"

/*
*/
COverhead::COverhead()
{
	memset(this, 0x00, sizeof(COverhead));
}

/*
*/
COverhead::COverhead(CVideoDevice* videoDevice, CErrorLog* errorLog, CColor* color, CShaderManager* shaderManager, CVertex2 size, CVertex2 position)
{
	memset(this, 0x00, sizeof(COverhead));

	m_videoDevice = videoDevice;

	m_errorLog = errorLog;

	m_color = color;

	m_size = size;

	m_position = position;


	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i] = m_videoDevice->CreateCommandAllocator();

		m_renderTargets[i] = new CRenderTarget(m_errorLog, m_videoDevice->m_device);

		m_renderTargets[i]->AdditionalTarget(m_resources[i], (UINT64)size.p.x, (UINT)size.p.y);

		m_depthBuffers[i] = new CDepthBuffer(m_errorLog, m_videoDevice->m_device,
			(UINT)size.p.x, (UINT)size.p.y);
	}

	m_commandList = m_videoDevice->CreateCommandList(m_commandAllocators[0]);

	m_commandList->SetName(L"COverhead::COverhead");

	m_commandList->Close();


	m_texture = new CDynamicTexture(m_errorLog, m_videoDevice, m_commandList,
		256, 256,
		"CDynamicTexture::Overhead");


	float xScale = (m_videoDevice->m_swapChainViewport->m_viewport.Width > 0) ?
		2.0f / m_videoDevice->m_swapChainViewport->m_viewport.Width : 0.0f;
	float yScale = (m_videoDevice->m_swapChainViewport->m_viewport.Height > 0) ?
		2.0f / m_videoDevice->m_swapChainViewport->m_viewport.Height : 0.0f;

	XMMATRIX wvpMat
	(
		xScale, 0, 0, 0,
		0, -yScale, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	);

	m_wvp = new CFloat4x4Buffer(m_videoDevice, m_errorLog, m_commandList);

	m_floats = new CFloatBuffer(m_videoDevice, m_errorLog, m_commandList);

	m_ints = new CIntBuffer(m_videoDevice, m_errorLog, m_commandList);

	
	XMStoreFloat4x4(&m_wvp->m_values[0], wvpMat);



	m_shaderHeap = new CShaderHeap(m_videoDevice, m_errorLog, m_commandList, 4);

	// These resources must be mapped to the pixel shader buffers
	m_shaderHeap->SlotResource(0, m_wvp);
	m_shaderHeap->SlotResource(1, m_floats);
	m_shaderHeap->SlotResource(2, m_ints);
	m_shaderHeap->SlotResource(3, m_texture);

	m_floats->m_values[3] = 1.0f;


	m_range = new CRange();

	// the number three represents the number of constant buffers
	m_range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3);

	// the number one represents the number of texture buffers
	m_range->SetRange(1, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

	// the root signature defines the number of samplers
	m_rootSignature = new CRootSignature(m_videoDevice, m_errorLog, 2, m_range);


	m_vertexShader = shaderManager->Create("vertexImage.hlsl", "VSMain", "vs_5_0");
	m_pixelShader = shaderManager->Create("pixelImage.hlsl", "PSMain", "ps_5_0");


	m_pipelineState = new CPipelineState(m_videoDevice, m_errorLog, CVertex::E_VT_VERTEXT);

	m_pipelineState->Create(false, true, false,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		m_vertexShader, m_pixelShader, nullptr, nullptr, nullptr,
		m_rootSignature->m_signature);


	m_vertices[0].p.x = 0.0f;
	m_vertices[0].p.y = 0.0f;

	m_vertices[0].uv.x = 0.0f;
	m_vertices[0].uv.y = 0.0f;

	m_vertices[1].p.x = m_size.p.x;
	m_vertices[1].p.y = 0.0f;

	m_vertices[1].uv.x = 1.0f;
	m_vertices[1].uv.y = 0.0f;

	m_vertices[2].p.x = m_size.p.x;
	m_vertices[2].p.y = m_size.p.y;

	m_vertices[2].uv.x = 1.0f;
	m_vertices[2].uv.y = 1.0f;

	m_vertices[3].p.x = 0.0f;
	m_vertices[3].p.y = m_size.p.y;

	m_vertices[3].uv.x = 0.0f;
	m_vertices[3].uv.y = 1.0f;

	m_vertexBuffer = new CVertexBuffer(m_videoDevice, m_errorLog, m_commandList, CVertex::E_VT_VERTEXT, 4, (void*)m_vertices);


	DWORD index[6] = {};

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	m_indexBuffer = new CIndexBuffer(m_videoDevice, m_errorLog, m_commandList, 6, index);

	COverhead::UpdatePosition();

	m_viewport = new CViewport(0.0f, 0.0f, m_size.p.x, m_size.p.y);

	m_camera = new CCamera(m_size.p.x, m_size.p.y, (CVertex*)&m_position, 45.0f, 1.0f, 50000.0f);

	m_camera->m_position.y = 128.0f;
	m_camera->UpdateRotation(90.0f, 0.0F, 0.0f);
	m_camera->UpdateView();
}

/*
*/
COverhead::~COverhead()
{
	delete m_camera;
	delete m_viewport;
	delete m_indexBuffer;
	delete m_vertexBuffer;
	delete m_pipelineState;
	delete m_rootSignature;
	delete m_range;
	delete m_shaderHeap;
	delete m_ints;
	delete m_floats;
	delete m_wvp;
	delete m_texture;

	m_commandList.Reset();

	for (int i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		delete m_depthBuffers[i];

		m_resources[i].Reset();

		delete m_renderTargets[i];

		m_commandAllocators[i].Reset();
	}
}

/*
*/
void COverhead::ClearTargets()
{
	m_videoDevice->m_commandList->ClearRenderTargetView(
		m_renderTargets[m_videoDevice->m_backbufferIndex]->m_handle,
		m_color->LovelyPurple->GetFloatArray(),
		0,
		nullptr);
	
	m_videoDevice->m_commandList->ClearDepthStencilView(
		m_depthBuffers[m_videoDevice->m_backbufferIndex]->m_handle,
		D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr);
}

/*
*/
void COverhead::Record()
{
	m_commandAllocators[m_videoDevice->m_backbufferIndex]->Reset();

	m_commandList->Reset(m_commandAllocators[m_videoDevice->m_backbufferIndex].Get(), nullptr);


	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.pResource =
		m_renderTargets[m_videoDevice->m_backbufferIndex]->m_resource.Get();
	
	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.StateBefore =
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
	
	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.StateAfter =
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;

	m_commandList->ResourceBarrier(1, &m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier);

	m_commandList->CopyResource(m_texture->m_texture.Get(),
		m_renderTargets[m_videoDevice->m_backbufferIndex]->m_resource.Get());

	m_texture->m_barrier.Transition.pResource = m_texture->m_texture.Get();
	m_texture->m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	m_texture->m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	m_commandList->ResourceBarrier(1, &m_texture->m_barrier);


	m_commandList->RSSetViewports(1, &m_videoDevice->m_swapChainViewport->m_viewport);

	m_commandList->RSSetScissorRects(1, &m_videoDevice->m_swapChainViewport->m_scissorRect);

	m_commandList->OMSetRenderTargets(1,
		&m_videoDevice->m_swapChainRenderTargets[m_videoDevice->m_backbufferIndex]->m_handle,
		false,
		&m_videoDevice->m_swapChainDepthBuffers[m_videoDevice->m_backbufferIndex]->m_handle);


	m_commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_commandList->SetGraphicsRootSignature(m_rootSignature->m_signature.Get());

	m_commandList->SetPipelineState(m_pipelineState->m_pipelineState.Get());


	m_wvp->UpdateConstantBuffer();

	m_floats->UpdateConstantBuffer();

	m_ints->UpdateConstantBuffer();


	m_commandList->SetDescriptorHeaps(1, m_shaderHeap->m_heap.GetAddressOf());

	m_commandList->SetGraphicsRootDescriptorTable(0, m_shaderHeap->m_heap->GetGPUDescriptorHandleForHeapStart());


	m_indexBuffer->Draw();

	m_vertexBuffer->DrawIndexed(m_indexBuffer->m_count);


	m_texture->m_barrier.Transition.pResource = m_texture->m_texture.Get();
	m_texture->m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	m_texture->m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;

	m_commandList->ResourceBarrier(1, &m_texture->m_barrier);


	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.pResource =
		m_renderTargets[m_videoDevice->m_backbufferIndex]->m_resource.Get();

	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.StateBefore =
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;

	m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier.Transition.StateAfter =
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_commandList->ResourceBarrier(1, &m_renderTargets[m_videoDevice->m_backbufferIndex]->m_barrier);
}

/*
*/
void COverhead::SetCameraPosition(XMFLOAT3 position)
{
	m_camera->m_position = position;

	m_camera->m_position.y = position.y + 32.0f;
}

/*
*/
void COverhead::UpdatePosition()
{
	m_vertices[0].p.x = m_position.p.x;
	m_vertices[1].p.x = m_position.p.x + m_size.p.x;
	m_vertices[2].p.x = m_position.p.x + m_size.p.x;
	m_vertices[3].p.x = m_position.p.x;

	m_vertices[0].p.y = m_position.p.y;
	m_vertices[1].p.y = m_position.p.y;
	m_vertices[2].p.y = m_position.p.y + m_size.p.y;
	m_vertices[3].p.y = m_position.p.y + m_size.p.y;

	if ((m_vertices[1].p.x < 0.0f) || (m_vertices[3].p.y < 0.0f) ||
		(m_vertices[0].p.x > m_videoDevice->m_width) ||
		(m_vertices[1].p.y > m_videoDevice->m_height))
	{
		return;
	}

	m_vertexBuffer->LoadBuffer((void*)m_vertices);
}