class Mover : IEntity
{
    Entity self;

	Entity ground;
	Entity camera;

    float ground_start_pos_y = 0;
    float time = 0;

    void Start() {
		ground = Scene::find_by_name("Ground");
		camera = Scene::find_by_name("Camera");
        ground_start_pos_y = ground.get_position_y();
    }

    void Update(float dt) {
        time += dt * 5;
        ground.set_position_y(ground_start_pos_y + sin(time) * 0.2);
		camera.set_position_x(sin(time) * 0.2);
		camera.set_rotation(time * 3.0);
    }
}
