# MinerSoftRender
重点3个类，
模型类model，记录了一个模型的mesh信息、贴图信息、位移旋转缩放等。
场景类scene，scene中记录了场景中的光照信息、camera和多个model。
相机类camera，记录了相机的位置和朝向。
渲染的时候，先从camera中计算出vp矩阵，然后对scene中的每个model,
1.绑定它的mesh和贴图
2.通过设置uniform的方式把光照信息传到shader中去，
3.从模型的位移旋转缩放信息得出m矩阵，乘以vp矩阵得到mvp矩阵，
4.通过设置uniform的方式把mvp矩阵传到shader中去
5.调用draw命令来绘制这个模型
